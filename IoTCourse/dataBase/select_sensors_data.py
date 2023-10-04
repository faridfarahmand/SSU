import mysql.connector
import pandas as pd

db = mysql.connector.connect(host="localhost", user="admin", password="mahir", database="sensors")

db.connect()
cursor = db.cursor()

sql = 'select * from sensors'

sql_result = pd.read_sql_query(sql, db)
df = pd.DataFrame(sql_result)
print(df)
