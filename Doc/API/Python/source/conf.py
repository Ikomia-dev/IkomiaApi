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
sys.path.insert(0, os.path.abspath('../../../../Build/Lib/Python'))


# -- Project information -----------------------------------------------------

project = 'Ikomia'
copyright = '2020, Ikomia SAS'
author = 'Ludovic Barusseau'

# The full version, including alpha/beta/rc tags
version = '0.3.0'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.coverage',
    'sphinx.ext.napoleon',
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
html_theme = 'alabaster'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Auto-summary ON/OFF
autosummary_generate = True

# Style customization
html_theme_options = {
    'description': 'A shared vision of research',
    'fixed_sidebar': 'true',
    'logo': 'app.png',
    'logo_name': 'true',
    'logo_text_align': 'center',
    'github_button': 'true',
    'github_user': 'Ikomia-dev',
    'github_repo': '/',
    'extra_nav_links': {
        'Website': 'https://ikomia.com',
        'Blog': 'https://blog.ikomia.com'},
    'show_powered_by': 'false',
    'show_relbars': 'true',

    #colors
    'link': '#cc5a20',
    'pre_bg': '#fcf0ea',
    'link_hover': '#944117',
    'sidebar_header': '#303030',
    'sidebar_link': '#cc5a20',
    'font_size': '15px',
    'sidebar_width': '250px'
}
