import csv,os
from paths import *
from datetime import datetime,timedelta
import shutil

# Sample 01 Fields
# FIELDS = ["Inside-Tempurature","Inside-Humidity","Outside-Tempurature","Outside-Humidity"]
#Sample 02 Fields
FIELDS = ["Time Stamp","Battery Level","isCharging","Temp In","Temp Out","Hum In","Hum Out"]
# Sample 01 CSV File
# RAW_CSV = os.path.join(RAW_DATA_PATH, 'data.csv')
# Sample 02 CSV File
RAW_CSV = os.path.join(SAMPLE_03_PATH, 'data.csv')
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

    return processed_data[2:]


def write_csv_file(file_name:str, data:list)->None:
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

data_list  = process_csv_file(RAW_CSV)
starting_date = datetime.strptime(data_list[0]['Time Stamp'], '%Y-%m-%d %H:%M:%S')
# print(starting_date)
# last_date = starting_date
# for data in data_list[1:2]:
#     new_date = last_date + timedelta(minutes=5,seconds=15)
#     shutil.copyfile(os.path.join(SAMPLE_03_PATH,data['Time Stamp'].replace(' ','-').replace(':','-')+".txt"),os.path.join(PREPROCESSED_TEXT_FILES_PATH,new_date.strftime('%Y-%m-%d-%H-%M-%S')+".txt"))
#     data['Time Stamp'] = new_date.strftime('%Y-%m-%d %H:%M:%S')
#     last_date = new_date

preprossed_csv_path = os.path.join(PREPROCESSED_CSV_FILES_PATH, 'sample-03.csv')
write_csv_file(preprossed_csv_path, data_list)
