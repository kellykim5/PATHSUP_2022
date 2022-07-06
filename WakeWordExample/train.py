import numpy as np
from glob import glob
from os.path import basename

def load_features(folder):
    dataset = None
    classmap = {}
    for class_idx, filename in enumerate(glob('%s/*.csv' % folder)):
        class_name = basename(filename)[:-4]
        classmap[class_idx] = class_name
        samples = np.loadtxt(filename, dtype=float, delimiter=',')
        labels = np.ones((len(samples), 1)) * class_idx
        samples = np.hstack((samples, labels))
        dataset = samples if dataset is None else np.vstack((dataset, 
samples))

    return dataset, classmap

from sklearn.ensemble import RandomForestClassifier
import sys
!{sys.executable} -m pip install micromlgen
from micromlgen import port

# put your samples in the dataset folder
# one class per file
# one feature vector per line, in CSV format
features, classmap = load_features('dataset/')
X, y = features[:, :-1], features[:, -1]
classifier = RandomForestClassifier(n_estimators=30, max_depth=10).fit(X, 
y)
c_code = port(classifier, classmap=classmap)
print(c_code)
with open('model.h', 'w') as f:
    f.write(c_code)