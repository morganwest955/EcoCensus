# Dummy standin file for predictions

import os;

def predictions(rootdir):
	count = 0
	path = os.path.dirname(rootdir)
	files = os.listdir(path)
	count = len(files)
	return count
