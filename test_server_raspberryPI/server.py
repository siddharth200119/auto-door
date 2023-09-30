from flask import Flask, request
import sys

app = Flask(__name__)

@app.route('/sidd_room/motion_detection', methods=['POST'])
def motion_detection():
    if request.method == 'POST':
        print(request.json, file=sys.stderr)
        return(request.json)

if __name__ =='__main__':  
    app.run(port = 6969) 