# Testing

## General Structure

Testing and unit-tests are to be done on a file-by-file basis.
That means that for every `<name>.h` file in `./src/` there should be a `testing_<name>.c` file and an accompanying `test_<name>.py` file to be ran with pytest.

## Setting up testing

To set up a python environment to run the tests follow the below steps:
- Create a python environment `python3 -m venv env`.
- Activate python environment `source ./env/bin/activate`.
- Install pytest module with `pip3 install pytest`.
- in the current directory (`./testing/` from top level directory) run pytest with `pytest`.

Alternatively you can run `make test` to install set up, activate, and install the required python environment then run `pytest`, but this is not gaurunteed to work and the above process is more reliable.
