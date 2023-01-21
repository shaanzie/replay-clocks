import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

from Notebooks.log_parser import LogParser

parser = LogParser()
df = parser.df

epsilon = 10000
interval = 1000
alpha = 5
delta = 1000
# for epsilon in range(5000, 25000, 5000):
# for delta in range(1000, 10000, 1000):
for interval in range(1000, 10000, 1000):
# for alpha in range(5, 50, 5):
                sample_df = df[(df['epsilon'] == epsilon) & (df['delta'] == delta) & (df['interval'] == interval) & (df['alpha'] == alpha)]
                if len(sample_df.index) == 0:
                    continue
                print(
                    'epsilon: {epsilon}, delta: {delta}, interval: {interval}, alpha: {alpha}, max_epoch: {max_epoch}, max_offset_size: {max_offset_size}, mean_offset_size: {mean_offset_size}, max_counter: {max_counter}, max_counter_size: {max_counter_size}, mean_counter_size: {mean_counter_size}'
                    .format(
                        epsilon = epsilon,
                        delta = delta,
                        interval = interval,
                        alpha = alpha,
                        max_epoch = sample_df['f.epoch'].max(),
                        max_offset_size = sample_df['f.offset_size'].max(),
                        mean_offset_size = sample_df['f.offset_size'].mean(),
                        max_counter = sample_df['f.max_counter'].max(),
                        max_counter_size = sample_df['f.counter_size'].max(),
                        mean_counter_size = sample_df['f.counter_size'].mean()
                    )
                )