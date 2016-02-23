import maya.OpenMaya as om
import maya.cmds as mc

class Transform(object):
	def __init__(self):
		self.full_name = ''
		self.maya_node = None
		self.children = []
		#
		self.world_pos = [0, 0, 0]
		self.local_pos = [0, 0, 0]
		self.rotation = [0, 0, 0, 1]
		self.scale = [1, 1, 1]

class KasMdl(object):
		def __init__(self):
			self.__xforms = {} # Full path name vs. Transform class
		#end

		def __get_dag_root(self):
			root = None
			it = om.MItDependencyNodes()
			it.reset(om.MFn.kWorld)
			while not it.isDone():
				root = om.MFnDagNode(it.thisNode())
				it.next()
			return root
		#end

		def __get_xform_data(self, xform):
			maya_xform = om.MFnTransform(xform.maya_node.object())

			maya_pos = maya_xform.translation(om.MSpace.kObject)
			xform.local_pos = [maya_pos.x, maya_pos.y, maya_pos.z]

			maya_rotation = om.MQuaternion()
			maya_xform.getRotation(maya_rotation, om.MSpace.kObject)
			xform.rotation = [maya_rotation.x, maya_rotation.y, maya_rotation.z, maya_rotation.w]

			scale_util = om.MScriptUtil()
			scale_util.createFromList([0.0, 0.0, 0.0], 3)
			scale_ptr = scale_util.asDoublePtr()
			maya_xform.getScale(scale_ptr)
			xform.scale = [om.MScriptUtil().getDoubleArrayItem(scale_ptr, 0),
										 om.MScriptUtil().getDoubleArrayItem(scale_ptr, 1),
										 om.MScriptUtil().getDoubleArrayItem(scale_ptr, 2)]

			 gnergniertniortinrit
			 # todo: get world pos (center attrib???)
			xform.world_pos = [1, 1, 1]
		#end

		# Gets all child xforms recursively of the given xform.
		def __get_child_xforms(self, xform):
			for child_idx in range(xform.maya_node.childCount()):
				maya_child = xform.maya_node.child(child_idx)
				# Ignore manipulators and nodes we don't care about
				if maya_child.hasFn(om.MFn.kManipulator3D) or (not maya_child.hasFn(om.MFn.kTransform)):
					continue
				maya_dag_node = om.MFnDagNode(maya_child)
				child_node = Transform()
				child_node.full_name = maya_dag_node.fullPathName()
				child_node.maya_node = maya_dag_node
				self.__get_xform_data(child_node)
				xform.children.append(child_node)
				self.__get_child_xforms(child_node)
			#endfor
		#end

		def __debug_print_xform(self, xform, spacing):
			print ('-' * spacing),
			print '%s (%d)' % (xform.full_name, len(xform.children))

			print ('-' * (spacing+2)),
			print 'wpos(%f, %f, %f)' % (xform.world_pos[0], xform.world_pos[1], xform.world_pos[2])

			print ('-' * (spacing+2)),
			print 'lpos(%f, %f, %f)' % (xform.local_pos[0], xform.local_pos[1], xform.local_pos[2])

			print ('-' * (spacing+2)),
			print 'rot(%f, %f, %f, %f)' % (xform.rotation[0], xform.rotation[1], xform.rotation[2], xform.rotation[3])

			print ('-' * (spacing+2)),
			print 'scale(%f, %f, %f)' % (xform.scale[0], xform.scale[1], xform.scale[2])

			for curr_child in xform.children:
				self.__debug_print_xform(curr_child, spacing+spacing)
		#end

		def export(self):
			# Get the DAG root node
			dag_root = self.__get_dag_root()
			if dag_root == None:
				mc.warning('Failed to retrieve DAG root node.')
				return

			# Create an entry by hand for the root node
			root_node = Transform()
			root_node.full_name = "root"
			root_node.maya_node = dag_root

			# Get all children from the root
			self.__get_child_xforms(root_node)

			self.__debug_print_xform(root_node, 2)
		#end
#end

km = KasMdl()
km.export()
