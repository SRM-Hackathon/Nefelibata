import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# required packages
# !pip install firebase
# !pip install python_jwt
# !pip install gcloud
# !pip install sseclient
# !pip install pycrypto
# !pip install requests_toolbelt


##Data preprocessing----------------------------------
#upload the dataset

# #to import dataset to pwd
# from google.colab import files
# files.upload()

'''            data preprocessing of raw data
#convert and save file as a csv
da = pd.read_excel("Dataset.xlsx")
da.to_csv("datasett.csv")
dataset = pd.read_csv("123.csv")

#cleaning data
df = dataset[['BPM','RAW DATA']]
df.rename(columns={'RAW DATA':'Status'},inplace=True)

#manually setting labels
#df.Status[x:x]=#label

#final copy of csv
df.to_csv("123.csv")'''

#dataset
dataset = pd.read_csv("123.csv")
X = dataset.iloc[:,1].values
Y = dataset.iloc[:,2].values

#train test split
from sklearn.model_selection import train_test_split
x_train, x_test, y_train, y_test = train_test_split(X,Y,train_size = 0.8,random_state = 0)

#scaling
from sklearn.preprocessing import StandardScaler
sc_X = StandardScaler()
x_train = sc_X.fit_transform(x_train.reshape(-1,1))
x_test = sc_X.transform(x_test.reshape(-1,1))


#Training model using SVM with kernel rbf
from sklearn.svm import SVC
classifier_svm_rbf= SVC(kernel="rbf",random_state=0)
classifier_svm_rbf.fit(x_train,y_train)

#predicted value
y_pred_srbf = classifier_svm_rbf.predict(x_test)

#confusion matrix
from sklearn.metrics import confusion_matrix
cm_srbf = confusion_matrix(y_test,y_pred_srbf)


#accuracy with f1
from sklearn import metrics as m
m.f1_score(y_test,y_pred_srbf)

#establishing conection with firebase
from firebase import firebase
firebase = firebase.FirebaseApplication("https://heart-7e56b.firebaseio.com/",None)
 
#default values

data_heart_rt_bpm = { 'BPM':0}
data_heart_rt_Threshold = {'threshold':0}
pred_ls = []

#initialize with 0
firebase.patch("/heart_rt",data_heart_rt_bpm)
firebase.patch("/heart_rt",data_heart_rt_Threshold)


for i in range(10000):
	#get bpm
	get_bpm = firebase.get('/heart_rt/BPM','')	
	pred_ls.append(get_bpm)

	#predict threshold
	bpm_threshold = classifier_svm_rbf.predict(sc_X.transform(np.array(pred_ls[i]).reshape(-1,1)))

	#updating the threshold values
	res_threshold = firebase.put('/heart_rt/','threshold',int(bpm_threshold))

