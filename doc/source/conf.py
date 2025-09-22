# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('../..'))

# -- Project information -----------------------------------------------------

project = 'Ikomia API'
copyright = '2025, Ikomia SAS'
author = 'Ludovic Barusseau'

# The full version, including alpha/beta/rc tags
version = '0.15.0'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.coverage',
    'sphinx.ext.napoleon',
    'sphinx.ext.autosectionlabel',
    'sphinx_copybutton',
    'sphinx_inline_tabs',
    'myst_parser',
    'sphinxcontrib.mermaid'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'furo'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# html_css_files = ['ikomia.css', ]

html_logo = '_static/app.png'

# Auto-summary ON/OFF
autosummary_generate = False

# Style customization - Alabaster
html_theme_options = {
    "light_css_variables": {
        # "color-sidebar-caption-text": "#CC5A20",
        # "font-stack": "_static/Avenue Mono.woff2", # Regular text
        # "font-stack--monospace": "_static/n27-regular-webfont.ttf", # Code blocks + inline code
    },
}

pygments_style = "friendly"
pygments_dark_style = "native"
