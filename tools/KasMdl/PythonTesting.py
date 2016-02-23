import maya.OpenMaya as om

class ExporterTest(object):
	def __init__(self):
		self.__meshes = [] # mesh obj
		self.__xform_mesh_map = {} # transform obj -> mesh obj
		self.__mesh_transform_xforms = {} # transform obj -> [[posx, posy, posz], [...]]
	#end

	def __get_meshes(self):
		it = om.MItDag(om.MItDag.kDepthFirst, om.MFn.kMesh)
		while not it.isDone():
			# Ignore instances that are already in the list
			if it.isInstanced():
				if it.currentItem() not in self.__meshes:
					self.__meshes.append(it.currentItem())
				it.next()
				continue
			# Anything here is unique and non-instanced
			self.__meshes.append(it.currentItem())
			it.next()

		points = om.MFloatPointArray()
		om.MFnMesh(self.__meshes[1]).getPoints(points, om.MSpace.kObject)
		for idx in range(points.length()):
			print 'Point[%d]: (%f, %f, %f)' % (idx, points[idx].x, points[idx].y, points[idx].z)
	#end

	def __get_mesh_transforms(self):
		it = om.MItDag(om.MItDag.kDepthFirst, om.MFn.kTransform)
		while not it.isDone():
			# Bug in Maya: manips are included, so filter them out
			if it.currentItem().apiType() == om.MFn.kManipulator3D:
				it.next()
				continue

			# If a child node has a mesh, we care about this node
			xform_node = om.MFnTransform(it.currentItem())
			for child_idx in range(xform_node.childCount()):
				if xform_node.child(child_idx).hasFn(om.MFn.kMesh):
					self.__xform_mesh_map[it.currentItem()] = xform_node.child(child_idx)
			it.next()

		#for curr in self.__xform_mesh_map:
		#	xform_name = om.MFnTransform(curr).fullPathName()
		#	mesh_name = om.MFnMesh(self.__xform_mesh_map[curr]).fullPathName()
		#	print 'Xform(%s) has mesh(%s)' % (xform_name, mesh_name)
	#end

	def __get_mesh_transform_xforms(self):
		for curr_xform in self.__xform_mesh_map:
			# Convert to transform node
			xform_node = om.MFnTransform(curr_xform)

			# Call the xform command to make maya multiple the chain of matrices for us
			

			# Get 'center' plug and read it's position (worldspace even after freezing)
			#center_plug = xform_node.findPlug('center', False)
			#center = [center_plug.child(0).asFloat(), center_plug.child(1).asFloat(), center_plug.child(2).asFloat()]
			# Same for rotate
			#rotate_plug = om.MFnMatrixData((xform_node.findPlug('worldMatrix', False)[0]).asMObject()).transformation()
			#rot = rotate_plug.translation(om.MSpace.kWorld)
			#matrix = om.MFnMatrixData(rotate_plug.asMObject()).transformation()
			#rot = matrix.rotation()
			
			#rotation = [rotate_plug.child(0).asFloat(), rotate_plug.child(1).asFloat(), rotate_plug.child(2).asFloat()]

			#print 'Xform: ' + str(xform_node.fullPathName())
			#print 'Position: ' + str(center)
			#print 'Rotation: %f, %f, %f' % (rot.x, rot.y, rot.z)
			#print 'Rotation: ' + str(rotation)
	#end

	def __get_transforms(self):
		# note for below code: gets meshes that are not instanced.
		# reason for using this technique is for transforms mainly, so could still use MItDependencyNodes for meshes etc...
		# has to be MItDag as it gives a full dag path which is required for world space mspace arguments.
		xforms = []
		itdag = om.MItDag(om.MItDag.kDepthFirst, om.MFn.kTransform)
		while not itdag.isDone():
			if itdag.isInstanced():
				if itdag.currentItem() not in xforms:
					path = om.MDagPath()
					itdag.getPath(path)
					xforms.append(path)
				itdag.next()
				continue
			path = om.MDagPath()
			itdag.getPath(path)
			xforms.append(path)
			itdag.next()

		# The below correctly gets the position in world space!  I will probably have to compute local positions to the parent
		# based on the difference in world space positions like below.  Awkward but it will work.
		# I should also try just getting the world space of the rotation pivot, but test that moving the pivot in maya
		# doesn't affect it.  May be easier that way.
		for x in xforms:
			transform = om.MFnTransform(x)
			center = transform.findPlug('center', False)
			cx = center.child(0).asFloat()
			cy = center.child(1).asFloat()
			cz = center.child(2).asFloat()
			print '%s: (%f, %f, %f)' % (om.MFnTransform(x).fullPathName(), cx, cy, cz)

			#print '%s: (%d)' % (om.MFnTransform(x).fullPathName(), center)
			#pos = mat.getTranslation(om.MSpace.kWorld)
			#print '%s: (%f, %f, %f)' % (om.MFnTransform(x).fullPathName(), pos.x, pos.y, pos.z)
			

		print 'end'
		print ''

		it = om.MItDependencyNodes()
		it.reset(om.MFn.kTransform)
		while not it.isDone():
			# Ignore manipulators
			if it.thisNode().apiType() == om.MFn.kManipulator3D:
				it.next()
				continue

			# Create the Maya transform node for this node
			transform = om.MFnTransform(it.thisNode())
			# Loop through the children to decide what this transform node holds
			for curr_child in range(transform.childCount()):
				if transform.child(curr_child).hasFn(om.MFn.kMesh):
					# The transform parents a mesh
					self.__mesh_transforms.append(transform)
				elif transform.child(curr_child).hasFn(om.MFn.kLight):
					# The transform parents a light source
					self.__light_transforms.append(transform)

			# Add the transform regardless of its children, as something could rely on it for xform info
			self.__all_transforms.append(transform)
			it.next()
	#end

	def export(self):
		self.__get_meshes()
		self.__get_mesh_transforms()
		self.__get_mesh_transform_xforms()
	#end

#end

et = ExporterTest()
et.export()
