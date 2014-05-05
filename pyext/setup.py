from distutils.core import setup, Extension
import os

os.environ["CC"] = "g++"

spammodule = Extension("spam",
                       sources = ["spammodule.cpp"])

setup(name="SpamModule",
      version = "1.0",
      description = "Spam",
      ext_modules = [spammodule])

