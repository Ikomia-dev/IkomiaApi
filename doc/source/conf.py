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
copyright = '2020, Ikomia SAS'
author = 'Ludovic Barusseau'

# The full version, including alpha/beta/rc tags
version = '0.8.0'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.coverage',
    'sphinx.ext.napoleon',
    'sphinx_rtd_theme',
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
#html_theme = 'alabaster'
html_theme = 'sphinx_rtd_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

html_css_files = ['ikomia.css', ]

html_logo = '_static/app.png'

# Auto-summary ON/OFF
autosummary_generate = False

# Style customization - Alabaster
#html_theme_options = {
#    'description': 'A shared vision of research',
#    'fixed_sidebar': 'true',
#    'logo': 'app.png',
#    'logo_name': 'true',
#    'logo_text_align': 'center',
#    'github_button': 'true',
#    'github_user': 'Ikomia-dev',
#    'github_repo': '/',
#    'extra_nav_links': {
#        'Website': 'https://ikomia.com',
#        'Blog': 'https://blog.ikomia.com'},
#    'show_powered_by': 'false',
#    'show_relbars': 'true',

    #colors
#    'link': '#cc5a20',
#    'pre_bg': '#fcf0ea',
#    'link_hover': '#944117',
#    'sidebar_header': '#303030',
#    'sidebar_link': '#cc5a20',
#    'font_size': '15px',
#    'sidebar_width': '250px'
#}

# Style customization - ReadTHeDocs
html_theme_options = {
    #'analytics_id': 'UA-XXXXXXX-1',  #  Provided by Google in your dashboard
    #'analytics_anonymize_ip': False,
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'both',
    'style_external_links': False,
    'vcs_pageview_mode': 'blob',
    'style_nav_header_background': '#150E83',
    # Toc options
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 3,
    'includehidden': True,
    'titles_only': False
}
