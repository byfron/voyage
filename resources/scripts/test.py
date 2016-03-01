print 'Hello World2!'
db = script_context.executeSelect('SELECT * FROM ENTITIES')
script_context.sendMessage(db.getData(2,1))

