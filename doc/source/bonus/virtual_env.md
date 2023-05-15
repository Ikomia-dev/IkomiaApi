# How to create a virtual environment

### Creating a Virtual Environment

Before installing the Ikomia API, it's recommended to create a virtual environment. This helps to isolate your project's dependencies and avoid conflicts with other packages.

To create a virtual environment, follow these steps:

#### 1. Install the virtualenv package if you haven't already
```shell
pip install virtualenv 
```

#### 2. Create a new virtual environment in your selected directory
```shell
virtualenv my_ikomia_env
````
Replace my_ikomia_env with the name you'd like to give to your virtual environment.

#### 3. Activate the virtual environment

On Windows:
```shell
my_ikomia_env\Scripts\activate
````

On Linux or macOS:
```shell
source my_ikomia_env/bin/activate
````
Now that your virtual environment is active, you can proceed with the installation of the Ikomia API.