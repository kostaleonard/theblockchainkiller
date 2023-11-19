"""Processes command line arguments and runs the miner."""

from argparse import ArgumentParser
import os

PRIVATE_KEY_ENVIRONMENT_VARIABLE = "THEBLOCKCHAINKILLER_PRIVATE_KEY"
PUBLIC_KEY_ENVIRONMENT_VARIABLE = "THEBLOCKCHAINKILLER_PUBLIC_KEY"


def main():
    """Runs the program."""
    parser = ArgumentParser(
        description="Processes command line arguments and runs the miner."
    )
    parser.add_argument(
        "--private_key_file_contents",
        help="The contents of the private key file. If not supplied, the "
        " program will load the key contents from the "
        f"{PRIVATE_KEY_ENVIRONMENT_VARIABLE} environment variable.",
        required=False
    )
    parser.add_argument(
        "--public_key_file_contents",
        help="The contents of the public key file. If not supplied, the "
        " program will load the key contents from the "
        f"{PUBLIC_KEY_ENVIRONMENT_VARIABLE} environment variable.",
        required=False
    )
    args = parser.parse_args()
    if not args.private_key_file_contents:
        args.private_key_file_contents = os.environ[
            PRIVATE_KEY_ENVIRONMENT_VARIABLE
        ]
    if not args.public_key_file_contents:
        args.public_key_file_contents = os.environ[
            PUBLIC_KEY_ENVIRONMENT_VARIABLE
        ]
    # TODO launch the miner


if __name__ == "__main__":
    main()
