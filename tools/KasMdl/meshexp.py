import maya.OpenMaya as om
import maya.cmds as mc

class Xform(object):
	def __init__(self):
		self.name = ''
		self.maya_obj = None
		self.parent_obj = None
		self.children = []
		#
		self.local_position = [0.0, 0.0, 0.0]
		self.local_rotation = [0.0, 0.0, 0.0, 1.0]
		self.local_scale = [1.0, 1.0, 1.0]
		self.world_position = [0.0, 0.0, 0.0]
	#end
#end

class Vertex(object):
	def __init__(self):
		self.position = [0.0, 0.0, 0.0]
		self.normal = [0.0, 0.0, 0.0]
		self.texcoord = [0.0, 0.0]
	#end
#end

class Mesh(object):
	def __init__(self):
		self.name = ''
		self.maya_obj = None
		self.source_xform = ''
		self.vertices = []
		self.indices = []
	#end
#end

class KasMdl(object):
		def __init__(self):
			self.__all_xforms = []
			self.__xforms = {} # full path name (string) -> xform (Xform)
			self.__meshes = {} # full path name (string) vs. Mesh
			self.__xform_mesh_dict = {} # full xform path name (string) -> mfnmesh (MObject)
		#end

		def __get_xform_data(self, xform):
			# Get the maya transform node to get data from
			maya_xform = om.MFnTransform(xform.maya_obj)

			# Local position
			maya_local_pos = maya_xform.translation(om.MSpace.kObject)
			xform.local_position = [maya_local_pos.x, maya_local_pos.y, maya_local_pos.z]

			# Local rotation
			maya_rotation = om.MQuaternion()
			maya_xform.getRotation(maya_rotation, om.MSpace.kObject)
			xform.local_rotation = [maya_rotation.x, maya_rotation.y, maya_rotation.z, maya_rotation.w]

			# Local scale
			maya_scale_util = om.MScriptUtil()
			maya_scale_util.createFromList([0.0, 0.0, 0.0], 3)
			maya_scale_ptr = maya_scale_util.asDoublePtr()
			maya_xform.getScale(maya_scale_ptr)
			maya_scale_x = om.MScriptUtil().getDoubleArrayItem(maya_scale_ptr, 0)
			maya_scale_y = om.MScriptUtil().getDoubleArrayItem(maya_scale_ptr, 1)
			maya_scale_z = om.MScriptUtil().getDoubleArrayItem(maya_scale_ptr, 2)
			xform.local_scale = [maya_scale_x, maya_scale_y, maya_scale_z]

			# World position using the 'center' attribute
			maya_center_plug = maya_xform.findPlug('center', False)
			maya_center_x = maya_center_plug.child(0).asFloat()
			maya_center_y = maya_center_plug.child(1).asFloat()
			maya_center_z = maya_center_plug.child(2).asFloat()
			xform.world_position = [maya_center_x, maya_center_y, maya_center_z]
		#end

		def __build_xform_tree(self):
			it = om.MItDependencyNodes(om.MFn.kTransform)
			while not it.isDone():
				maya_obj = it.thisNode()
				# Ignore manipulators (bug with maya api... surprise!)
				if maya_obj.hasFn(om.MFn.kManipulator3D):
					it.next()
					continue
				#endif
				dag_node = om.MFnDagNode(maya_obj)

				# Xform for this node
				xform_node = Xform()
				xform_node.name = dag_node.fullPathName()
				xform_node.maya_obj = maya_obj

				# Compute data for this xform
				self.__get_xform_data(xform_node)

				# Get parent details (even top-level objects have a parent)
				parent_obj = om.MFnDagNode(dag_node.parent(0))
				parent_name = parent_obj.fullPathName()

				# If the parent exists, add it as a child node
				if parent_name in self.__xforms:
					xform_node.parent_obj = parent_obj
					self.__xforms[parent_name].children.append(xform_node)
				else:
					# Otherwise, add as a new node
					self.__xforms[xform_node.name] = xform_node

				# Add the current node to the list of all xforms
				self.__all_xforms.append(xform_node)
				it.next()
			#endwhile
		#end

		def __get_mesh_nodes(self):
			it = om.MItDependencyNodes()
			it.reset(om.MFn.kMesh)
			while not it.isDone():
				# Dag node of the mesh
				dag_node = om.MFnDagNode(it.thisNode())
				# Create a Mesh
				mesh_node = Mesh()
				mesh_node.name = dag_node.fullPathName()
				mesh_node.maya_obj = it.thisNode()
				# Get the first parent of the mesh as that is the source, even with instances.
				mesh_node.source_xform = om.MFnDagNode(dag_node.parent(0)).fullPathName()
				# Add the mesh to the dictionary
				self.__meshes[mesh_node.name] = mesh_node
				it.next()
			#endwhile
		#end

		def __link_xform_meshes(self):
			for curr in self.__all_xforms:
				dag_node = om.MFnDagNode(curr.maya_obj)
				for child_idx in range(dag_node.childCount()):
					child_obj = dag_node.child(child_idx)
					# If the child node has an mesh, link it to the xform
					if child_obj.hasFn(om.MFn.kMesh):
						self.__xform_mesh_dict[curr] = self.__meshes[om.MFnMesh(child_obj).fullPathName()]
						break
					#endif
				#endfor
			#endfor
		#end

		def __compute_mesh_data(self):
			for curr_mesh in self.__meshes:
				# print 'Processing mesh (%s)' % (curr_mesh)
				# Get the Maya mesh
				maya_mesh = om.MFnMesh(self.__meshes[curr_mesh].maya_obj)

				# Does the mesh have UVs?
				uv_set_names = []
				maya_mesh.getUVSetNames(uv_set_names)
				has_uvs = True if (len(uv_set_names) and (maya_mesh.numUVs(uv_set_names[0]) > 0)) else False

				# Iterate through all of the vertices and get their data
				it = om.MItMeshFaceVertex(self.__meshes[curr_mesh].maya_obj)
				while not it.isDone():
					# Vertex we will populate with data
					vert = Vertex()

					# Get the local position relative to the world origin (0, 0, 0)
					maya_pos = it.position(om.MSpace.kObject)
					vert.position = [maya_pos.x, maya_pos.y, maya_pos.z]

					# Get the normal
					maya_nrm = om.MVector()
					it.getNormal(maya_nrm, om.MSpace.kObject)
					vert.normal = [maya_nrm.x, maya_nrm.y, maya_nrm.z]

					# Texcoords
					if has_uvs and it.hasUVs():
						uv_util = om.MScriptUtil()
						uv_util.createFromList([0.0, 0.0], 2)
						uv_ptr = uv_util.asFloat2Ptr()
						it.getUV(uv_ptr)
						maya_u = om.MScriptUtil().getFloat2ArrayItem(uv_ptr, 0, 0)
						maya_v = om.MScriptUtil().getFloat2ArrayItem(uv_ptr, 0, 1)
						vert.texcoord = [maya_u, maya_v]
					#endif

					self.__meshes[curr_mesh].vertices.append(vert)
					it.next()
				#endwhile

				# Get all index data.
				mia1 = om.MIntArray()
				mia2 = om.MIntArray()
				maya_mesh.getTriangleOffsets(mia1, mia2)
				for idx in range(mia2.length()):
					self.__meshes[curr_mesh].indices.append(mia2[idx])
				#endfor
			#endfor
		#end

		def export(self):
			self.__build_xform_tree()
			self.__get_mesh_nodes()
			self.__link_xform_meshes()
			self.__compute_mesh_data()

			print 'All meshes:'
			for curr in self.__meshes:
				print '[%s]: %s' % (curr, self.__meshes[curr].name)

			for curr in self.__xform_mesh_dict:
				print 'Xform (%s) has mesh (%s)' % (curr.name, self.__xform_mesh_dict[curr].name)

		#end
#end

km = KasMdl()
km.export()
