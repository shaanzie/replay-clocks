import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys
import ast

file_path = sys.argv[1]

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

log_df = pd.read_csv(file_path, header = None, names = columns, sep=',', converters={1:ast.literal_eval})

print(
    '{epsilon},{delta},{interval},{alpha},{max_epoch},{max_offset_size},{mean_offset_size},{max_counter},{max_counter_size},{mean_counter_size}'
    .format(
        epsilon = log_df['epsilon'].iloc[0],
        delta = log_df['delta'].iloc[0],
        interval = log_df['interval'].iloc[0],
        alpha = log_df['alpha'].iloc[0],
        max_epoch = log_df['f.epoch'].max(),
        max_offset_size = log_df['f.offset_size'].max(),
        mean_offset_size = log_df['f.offset_size'].mean(),
        max_counter = log_df['f.max_counter'].max(),
        max_counter_size = log_df['f.counter_size'].max(),
        mean_counter_size = log_df['f.counter_size'].mean()
    )
)