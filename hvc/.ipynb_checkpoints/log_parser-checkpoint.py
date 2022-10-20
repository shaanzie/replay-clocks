import pandas as pd
import os
import ast

class LogParser:

    def __init__(self, log_path: str = 'LOGFILES') -> None:
        
        columns = [
            'from_node',
            'to_node',
            'time',
            'm.epoch',
            'm.offsets',
            'm.counters',
            'e.epoch',
            'e.offsets',
            'e.counters',
            'f.epoch',
            'f.offsets',
            'f.counters',
            'f.offset_size',
            'f.counter_size',
            'epsilon',
            'num_nodes',
            'msg_count',
            'send_threshold',
            'interval'
        ]
        os.chdir(log_path)
        self.df = pd.DataFrame(columns = columns)
        # iterate through all file
        for file in os.listdir():
            # Check whether file is in text format or not
            if file.endswith(".log"):
                file_path = f"/mnt/d/Code/summer_research/hybrid-vector-clocks/hvc/{log_path}/{file}"
                print(file_path)
        
                # call read text file function
                log_df = pd.read_csv(file_path, header = None, names = columns, sep=',', converters={1:ast.literal_eval})
                self.df = pd.concat([self.df, log_df])
                
    def format_lists(self):
        
        self.df = self.df['m.offsets'].str.split(',')
        self.df = self.df['m.counters'].str.split(',')
        self.df = self.df['e.offsets'].str.split(',')
        self.df = self.df['e.counters'].str.split(',')
        self.df = self.df['f.offsets'].str.split(',')
        self.df = self.df['f.counters'].str.split(',')
    
    def read_text_file(self, file_path):
        with open(file_path, 'r') as f:
            return f.read()
    
if __name__ == '__main__':

    parser = LogParser()
    # print(parser.df.head(1))
    parser.df.to_csv('logs.csv')
