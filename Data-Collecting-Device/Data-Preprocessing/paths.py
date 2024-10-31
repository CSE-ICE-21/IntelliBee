import os

PARENT_PATH = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA_PREPROCESSING_PATH = os.path.join(PARENT_PATH, 'Data-Preprocessing')
DATA_COLLECTED_PATH = os.path.join(PARENT_PATH, 'Data-Collected')
RAW_DATA_PATH = os.path.join(DATA_COLLECTED_PATH, 'Raw-Data')
SAMPLE_01_PATH = os.path.join(RAW_DATA_PATH, 'Sample-01')
SAMPLE_02_PATH = os.path.join(RAW_DATA_PATH, 'Sample-02')
PREPROCESSED_DATA_PATH = os.path.join(DATA_COLLECTED_PATH, 'Preprocessed-Data')
PREPROCESSED_AUDIO_FILES_PATH = os.path.join(PREPROCESSED_DATA_PATH, 'Audio-Files')
PREPROCESSED_CSV_FILES_PATH = os.path.join(PREPROCESSED_DATA_PATH, 'CSV-Files')
PREPROCESSED_TEXT_FILES_PATH = os.path.join(PREPROCESSED_DATA_PATH, 'Text-Files')