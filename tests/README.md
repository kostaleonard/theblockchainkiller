# Test suite notes

CMake builds the test suite into an executable located at build/tests.
Running this executable launches the test suite.
You may also wish to run the executable with valgrind to check for memory leaks.

## Local configuration

Test functions that use cryptographic keys require additional configuration to run correctly.
You need to generate a public/private key pair and store them in environment variables.

First, generate a public/private key pair.

```sh
openssl genpkey -algorithm RSA -out private_key.pem
openssl rsa -pubout -in private_key.pem -out public_key.pem
```

We store the key pair as environment variables rather than files to maintain a consistent configuration with the CI/CD pipeline and prevent the keys from getting stored in version control.
Environment variables cannot contain newline characters.
However, the OpenSSL key format does contain newline characters.
We can base64 encode the key contents to remove newlines.

```sh
base64 -w 0 public_key.pem
base64 -w 0 private_key.pem
```

Store these values in the `THEBLOCKCHAINKILLER_TEST_PUBLIC_KEY` and `THEBLOCKCHAINKILLER_TEST_PRIVATE_KEY` environment variables.
On Windows, you may need to use `regedit` to set the environment variables because the Control Panel sets an artificially low character limit on environment variables.

## Remote (GitHub) configuration

We have already configured the CI/CD pipeline.
However, you may need to make adjustments.
The CI/CD pipeline has an identical configuration to local, but you provide the environment variables through CI Variables.

Follow the steps in [Local configuration](#local-configuration) to generate a public/private key pair and base64 encode them.
Then save the base64 strings to CI Variables with the names `THEBLOCKCHAINKILLER_TEST_PUBLIC_KEY` and `THEBLOCKCHAINKILLER_TEST_PRIVATE_KEY`.
CI runners will have these environment variables.
