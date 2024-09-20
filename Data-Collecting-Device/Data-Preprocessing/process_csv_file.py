import csv,os
from paths import *

FIELDS = ["Inside-Tempurature","Inside-Humidity","Outside-Tempurature","Outside-Humidity"]
RAW_CSV = os.path.join(RAW_DATA_PATH, 'data.csv')
PREPROCESSED_CSV = os.path.join(PREPROCESSED_CSV_FILES_PATH, 'data.csv')

def process_csv_file(file_name:str)->dict:
    """
    This function processes the CSV file and returns the processed data in the form of a Dictionary.
    :param file_name: Name of the CSV file to be processed.
    :return: Dictionary containing the processed data.
    """
    processed_data = []

    with open(RAW_CSV, 'r') as file:
        csv_dict_reader = csv.DictReader(file, fieldnames=FIELDS)
        for row in csv_dict_reader:
            processed_data.append(row)

    return processed_data


def write_csv_file(file_name:str, data:dict)->None:
    """
    This function writes the processed data to a CSV file.
    :param file_name: Name of the CSV file to be written.
    :param data: Processed data to be written to the CSV file.
    """
    with open(file_name, 'w') as file:
        csv_dict_writer = csv.DictWriter(file, fieldnames=FIELDS)
        csv_dict_writer.writeheader()
        for row in data:
            csv_dict_writer.writerow(row)

write_csv_file(PREPROCESSED_CSV, process_csv_file(RAW_CSV))