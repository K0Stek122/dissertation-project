import argparse


def setup_arguments():
    parser = argparse.ArgumentParser(description="Dataset Passthrough Container")
    
    parser.add_argument(
        "-i",
        "--input",
        type=str,
        required=True,
        help="Input dataset path."
    )
    
    # Argument that accepts a list of possible options
    parser.add_argument(
        "-d",
        "--dataset-type",
        type=str,
        choices=["UNSW-NB15", "CICIDS2017", "CAIDA2007"],
        required=True,
        help="Operation to perform on the dataset."
    )
    
    return parser.parse_args()

if __name__ == "__main__":
    args = setup_arguments()
    
    with open(args.input, 'r') as f:
        data = f.read()
    print(f"Dataset Type: {args.dataset_type}")
    print("Dataset Content:")
    print(data)
         