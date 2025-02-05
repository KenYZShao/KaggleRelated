#KaggleRelated Code from 2019 Oct, this comment is written in 2nd Dec 2021

import matplotlib.pyplot as plt
import numpy as np
from sklearn import datasets, linear_model
from sklearn.metrics import mean_squared_error, r2_score
import pandas as pd

# Load the target dataset of salary and age
dataset=pd.read_csv("dataset.csv")

filename = "dataset.csv"
# opening the file with w+ mode truncates the file
f = open(filename, "w+")
f.close()

from numpy import argmax
# define input string
dataset = 'hello world'
print(dd)
# define universe of possible input values
alphabet = 'abcdefghijklmnopqrstuvwxyz '
# define a mapping of chars to integers
char_to_int = dict((c, i) for i, c in enumerate(alphabet))
int_to_char = dict((i, c) for i, c in enumerate(alphabet))
# integer encode input data
integer_encoded = [char_to_int[char] for char in data]
print(integer_encoded)
# one hot encode
onehot_encoded = list()
for value in integer_encoded:
       letter = [0 for _ in range(len(alphabet))]
       letter[value] = 1
       onehot_encoded.append(letter)
print(onehot_encoded)
# invert encoding
inverted = int_to_char[argmax(onehot_encoded[0])]
print(inverted)

# Use only one feature
dataset_X = dd.data[:, np.newaxis, 2]

# Split the data into training(80%)/testing(20%) sets
dataset_train = dataset_X[111993:89595] 
dataset_test = dataset.target[111993:22398] 

dataset_train_x=dataset_train.loc[:, 'Age']
dataset_train_y=dataset_train.loc['Income in EUR',:]

# Substitute into Linear Regression model 

dataset_test_x=dataset_train.loc[:, 'Age']
#Substitute it into model to get predict_y
dataset_test_y=dataset_train.loc['Income in EUR',:]



dataset_X_test = dataset_X[111993:22398]

# Split the targets into training/testing sets

dataset_y_test = dataset.target[111993:22398]

# Create linear regression object
regr = linear_model.LinearRegression()

# Train the model using the training sets
regr.fit(dataset_X_train, dataset_y_train)

# Make predictions using the testing set
dataset_y_pred = regr.predict(dataset_X_test)

# The coefficients
print('Coefficients: \n', regr.coef_)
# The mean squared error
print("Mean squared error: %.2f"
      % mean_squared_error(dataset_y_test, dataset_y_pred))
# Explained variance score: 1 is perfect prediction
print('Variance score: %.2f' % r2_score(dataset_y_test, dataset_y_pred))

# Plot outputs
plt.scatter(dd_X_test, dd_y_test,  color='black')
plt.plot(dd_X_test, dd_y_pred, color='blue', linewidth=3)

plt.xticks(())
plt.yticks(())

plt.show()
