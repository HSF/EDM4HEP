"""Python module for the EDM4HEP datamodel and utilities."""

import sys

from .__version__ import __version__
import ROOT

res = ROOT.gSystem.Load("libedm4hepDict")
if res < 0:
    raise RuntimeError("Failed to load libedm4hepDict")

res = ROOT.gSystem.Load("libedm4hepRDF")
if res < 0:
    raise RuntimeError("Failed to load libedm4hepRDF")

res = ROOT.gInterpreter.LoadFile("edm4hep/utils/kinematics.h")
if res != 0:
    raise RuntimeError("Failed to load kinematics.h")

res = ROOT.gInterpreter.LoadFile("edm4hep/utils/dataframe.h")
if res != 0:
    raise RuntimeError("Failed to load dataframe.h")

res = ROOT.gInterpreter.LoadFile("edm4hep/Constants.h")
if res != 0:
    raise RuntimeError("Failed to load Constants.h")
from ROOT import edm4hep

from podio.pythonizations import load_pythonizations

load_pythonizations("edm4hep")

# Make TAB completion work for utils
setattr(edm4hep, "utils", edm4hep.utils)

# set package attributes for edm4hep
edm4hep.__version__ = __version__
edm4hep.__name__ = __name__
edm4hep.__spec__ = __spec__
edm4hep.__path__ = __path__
edm4hep.__file__ = __file__

# Make `import edm4hep` work
sys.modules["edm4hep"] = edm4hep
