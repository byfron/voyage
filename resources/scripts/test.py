print 'Hello World!'
db = script_context.executeSelect('SELECT * FROM ENTITIES')
print db.getData(0,1)
