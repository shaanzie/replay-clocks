import pandas as pd
import os
import ast

class LogParser:

    def __init__(self, log_path: str = 'LOGDIR', params: list = ['.log']) -> None:
        
        columns = [
            'from_node',
            'to_node',
            'm.epoch',
            'm.offsets',
            'm.offset_size',
            'm.counters',
            'm.counter_size',
            'e.epoch',
            'e.offsets',
            'e.offset_size',
            'e.counters',
            'e.counter_size',
            'f.epoch',
            'f.offsets',
            'f.offset_size',
            'f.counters',
            'f.counter_size',
            'f.max_counter',
            'epsilon',
            'interval',
            'alpha',
            'delta'
        ]
        os.chdir(f"/mnt/d/Code/summer_research/hybrid-vector-clocks/hvc_new/{log_path}")
        self.df = pd.DataFrame(columns = columns)
        # iterate through all file
        for file in os.listdir():
            # Check whether file is in text format or not
            for s in params:
                if s in file:
                    file_path = f"/mnt/d/Code/summer_research/hybrid-vector-clocks/hvc_new/{log_path}/{file}"
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
