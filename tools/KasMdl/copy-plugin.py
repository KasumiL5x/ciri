import os.path
import shutil

def copy_plugin():
	src = "./KasMdl/bin/Debug/KasMdl.nll.dll"
	dst = "E:/tools/maya/plug-ins/KasMdl.nll.dll"

	if not os.path.isfile(src):
		print 'Source file does not exist.'
		return

	try:
		shutil.copyfile(src, dst)
	except IOError, e:
		print 'Failed to copy file.  Maya may still be running.'
		return

	print 'Successfully copied file.'
		
copy_plugin()
raw_input()
