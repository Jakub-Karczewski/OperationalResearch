import pandas as pd
from IPython.display import display
import numpy as np
from pandas.plotting import table
import matplotlib.pyplot as plt
import dataframe_image as dfi

import nest_asyncio
nest_asyncio.apply()  # ← allow sync Playwright calls inside the existing loop

import pandas as pd
import dataframe_image as dfi

df = pd.read_csv("table_params.csv")
df['ratio'] = df['cost'] / df['exec_time']
df_sorted = df.sort_values(by='ratio', ascending=False)

df_sorted = df_sorted.drop(columns='ratio')

df_styled = df_sorted.head(30).reset_index(drop=True).style.background_gradient()

# Now this runs without complaint:
dfi.export(df_styled, "population50.png")