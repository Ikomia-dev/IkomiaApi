Naming convention
=================

Ikomia plugins must follow a quiet strict naming convention to be loaded correctly.

The Ikomia plugin loader use this convention to detect automatically required entry points. The rules are also introduced for reasons of homogeneity and clarity in the Marketplace.

    - main entry point in a file named my_plugin.py
    - process implementation in a file named my_plugin_process.py
    - widget implementation in a file named my_plugin_widget.py
    - lowercase only
    - use of character '_' as separator