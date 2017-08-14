## Helper library for week 3
###### PLEASE CITE DataSoc: Daniel Han-Chen if you are using this elsewhere.

### Specifically made for DataSoc AI Lecture Series.

import pandas as pd, copy

def read(data, target): 
    df = pd.read_csv(data)
    y = df.pop(target)
    return df, y

def process(data):
    nums = (data.dtypes!='O').index[data.dtypes!='O'].tolist()
    good_not_nums = ((data.dtypes=='O') & (data.count()>0.65*len(data))).index[(data.dtypes=='O')&(data.count()>0.65*len(data))].tolist()
    bad_not_nums = ((data.dtypes=='O') & (data.count()<=0.65*len(data))).index[(data.dtypes=='O')&(data.count()<=0.65*len(data))].tolist()
    not_nums = (data.dtypes=='O').index[data.dtypes=='O'].tolist()
    
    data[nums] = data[nums].fillna(data[nums].mean())
    data[good_not_nums] = data[good_not_nums].fillna(data[good_not_nums].mode().iloc[0])
    data[bad_not_nums] = data[bad_not_nums].fillna("Missing")
    
    objs = copy.copy(not_nums)
    for col in not_nums: 
        if data[col].nunique() > 10: objs.remove(col)
    
    cats = []
    for col in nums:
        if data[col].nunique() <= 10: cats.append(col)
    
    data = data[objs+nums]
    dummify = cats+objs
    
    data = pd.get_dummies(data, columns = dummify)
    return data

def get(data, target):
    data,y = read(data, target)
    data = process(data)
    return data,y

def classifier(data, y, model = "forest"):
    if model == "forest":
        from sklearn.ensemble import RandomForestClassifier as rfc
        est = rfc(n_estimators = 10, n_jobs=-1)

    elif model == "tree":
        from sklearn.tree import DecisionTreeClassifier as dtc
        est = dtc()
        
    elif model == "extra": 
        from sklearn.ensemble import ExtraTreesClassifier as etc
        est = etc(n_estimators = 10, n_jobs=-1)
        
    elif model == "logistic":
        from sklearn.linear_model import LogisticRegression as lr
        cases = y.nunique()
        if cases > 2: est = lr(solver = "newton-cg", multi_class = "multinomial")
        else: est = lr(n_jobs=-1)
            
    elif model == "svm":
        from sklearn.svm import SVC as svc
        est = svc()
        
    elif model == "boost":
        from sklearn.ensemble import GradientBoostingClassifier as gbc
        est = gbc(n_estimators = 10)
    
    elif model == "neural":
        from sklearn.neural_network import MLPClassifier as nnc
        est = nnc(max_iter = 10, learning_rate_init = 1);
        
    est.fit(data, y);
    return est

def regression(data, y, model = "forest"):
    if model == "forest":
        from sklearn.ensemble import RandomForestRegressor as rfc
        est = rfc(n_estimators = 10, n_jobs=-1)

    elif model == "tree":
        from sklearn.tree import DecisionTreeRegressor as dtc
        est = dtc()
        
    elif model == "extra": 
        from sklearn.ensemble import ExtraTreesRegressor as etc
        est = etc(n_estimators = 10, n_jobs=-1)
        
    elif model == "linear":
        from sklearn.linear_model import LinearRegression as lr
        cases = y.nunique()
        est = lr(n_jobs=-1)
            
    elif model == "svm":
        from sklearn.svm import SVR as svc
        est = svc()
        
    elif model == "boost":
        from sklearn.ensemble import GradientBoostingRegressor as gbc
        est = gbc(n_estimators = 10)
    
    elif model == "neural":
        from sklearn.neural_network import MLPRegressor as nnc
        est = nnc(max_iter = 10, learning_rate_init = 1);
        
    est.fit(data, y);
    return est

def predict(data, model): return model.predict(data)

## Helper library for week 3
###### PLEASE CITE DataSoc: Daniel Han-Chen if you are using this elsewhere.

### Specifically made for DataSoc AI Lecture Series.
