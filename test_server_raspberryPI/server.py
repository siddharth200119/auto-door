from flask import Flask, request

app = Flask(__name__)

@app.route('/sidd_room/motion_detection', methods=['POST'])
def motion_detection():
    if request.method == 'POST':
        print(request.json)

if __name__ =='__main__':  
    app.run(port = 6969) 