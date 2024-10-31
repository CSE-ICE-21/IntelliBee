from paths import *
import os
import numpy as np
import wave




def list_all_raw_txt_files(path:str)->list:
    """
    This function lists all the raw text files in the specified path.
    :param path: Path to the directory containing the raw text files.
    :return: List of all the raw text files in the specified path.
    """
    file_list:list = [file for file in os.listdir(path) if file.endswith('.txt')]
    return file_list if len(file_list) > 0 else None

def process_txt_file(file_name:str,path:str)->np.ndarray:
    """
    This function processes the raw text file and returns the processed data.
    :param file_name: Name of the raw text file to be processed.
    :return: Processed data.
    """
    try:
        with open(os.path.join(path, file_name), 'r') as file:
            data = file.read()
        data = data.split('\n')[1].strip(",")
        string_data = data.strip()
        if len(string_data) == 0:
            return None
        if len(string_data) > 8000:
            string_data = string_data[:8000]
            print(f'Warning: Truncating data to 8000 samples for {file_name}')
        audio_data:np.ndarray = np.array([int(sample.strip()) for sample in string_data.split(',')],dtype=np.int32)
        return audio_data
        
    except FileNotFoundError:
        print(f'File {file_name} not found in {path}')
        return None
    except IOError:
        print(f'Error reading file {file_name} in {path}')
        return None
    except ValueError as e:
        print(f'Error processing file {file_name} in {path}. Error: {e}')
        return None
    except Exception as e:
        print(f'Error reading file {file_name} in {path}. Error: {e}')
        return None
import struct

def write_wav_file(output_file: str, audio_data: np.ndarray, sample_rate: int = 1000) -> None:
    """
    This function writes 24-bit audio data to a WAV file.
    :param output_file: Name of the WAV file to be written.
    :param audio_data: 24-bit audio data stored in int32 format.
    :param sample_rate: Sample rate of the audio data.
    """
    if audio_data is None or len(audio_data) == 0:
        print(f'No audio data to write for {output_file}')
        return

    try:
        # Mask to get the 24 least significant bits and keep audio data as signed integers
        audio_data_24bit = audio_data & 0xFFFFFF  # Keep only 24 bits
        audio_data_24bit = audio_data_24bit.astype(np.int32)  # Ensure it's in int32 for further processing
        
        # Open a file in write mode as a WAV file
        with wave.open(output_file, 'wb') as wav_file:
            num_channels = 1
            sampwidth = 3  # 24-bit is 3 bytes
            num_frames = len(audio_data_24bit)
            
            wav_file.setnchannels(num_channels)
            wav_file.setsampwidth(sampwidth)
            wav_file.setframerate(sample_rate)
            
            # Convert each sample to 3 bytes and write to the WAV file
            for sample in audio_data_24bit:
                wav_file.writeframes(struct.pack('<i', sample)[1:4])  # Write 3 bytes from int32
            
        print(f'Successfully written 24-bit audio data to {output_file}')

    except FileExistsError:
        print(f'File {output_file} already exists. Skipping.')
    except IOError as e:
        print(f'IOError writing file {output_file}: {e}')
    except Exception as e:
        print(f'Error writing file {output_file}: {e}')

def write_wav_file(output_file:str, audio_data:np.ndarray, sample_rate:int=1000) -> None:
    """
    This function writes the audio data to a WAV file.
    :param output_file: Name of the WAV file to be written.
    :param audio_data: Audio data to be written to the WAV file.
    :param sample_rate: Sample rate of the audio data.
    """
    if audio_data is None or len(audio_data) == 0:
        print(f'No audio data to write for {output_file}')
        return
    try:
        audio_data_16bit = (audio_data << 4).astype(np.int16)
        
        with wave.open(output_file, 'wb') as wav_file:
            num_channels = 1  
            sampwidth = 2  
            num_frames = len(audio_data)
            
            wav_file.setnchannels(num_channels)
            wav_file.setsampwidth(sampwidth)
            wav_file.setframerate(sample_rate)
            wav_file.writeframes(audio_data_16bit.tobytes())
            
    except FileExistsError:
        print(f'File {output_file} already exists. Skipping.')
    except IOError as e:
        print(f'IOError writing file {output_file}: {e}')
    except Exception as e:
        print(f'Error writing file {output_file}: {e}')


SAMPLE_02_PATH = os.path.join(RAW_DATA_PATH, 'Sample-02')

text_files:list = list_all_raw_txt_files(SAMPLE_02_PATH)
total_processed_files:int = 0
total_files:int = len(text_files)

if text_files is None:
    print(f'No text files found in {SAMPLE_02_PATH}')
    exit(0)
else:
    print(f'Found {len(text_files)} text files in {SAMPLE_02_PATH}')
print(process_txt_file(text_files[0],SAMPLE_02_PATH))  
audio_data = process_txt_file(text_files[0],SAMPLE_02_PATH) 
name = 'sample' + '.wav'
write_wav_file(name, audio_data)

print(f'Processing text files...')
# for file in text_files:
#     print("-"*50)
#     print(f'Processing {file}...')
#     audio_data = process_txt_file(file,RAW_DATA_PATH)
#     if audio_data is None:
#         print(f'Error processing {file}. No data found in the file. Skipping...')
#         continue
#     print(f"Found {len(audio_data)} samples")
#     audi_file_path = os.path.join(PREPROCESSED_AUDIO_FILES_PATH, file.split('.')[0] + '.wav')
#     write_wav_file(audi_file_path, audio_data)
#     print(f'Processed {file} and saved as {file.split(".")[0]}.wav')
#     total_processed_files += 1


# print(f'Processed {total_processed_files} out of {total_files} text files.')