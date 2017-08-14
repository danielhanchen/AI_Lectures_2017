data, y = get("train.csv", "Survived")
model = classifier(data, y, model = "tree")
predict(data, model)