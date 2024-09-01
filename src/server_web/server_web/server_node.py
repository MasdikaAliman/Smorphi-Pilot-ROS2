import rclpy 
from rclpy.node import Node
from std_msgs.msg import String
from geometry_msgs.msg import Point
from std_msgs.msg import Int32
from flask import Flask, request, jsonify
from icecream import ic

import threading

# ip_address = "192.168.100.164" #Our IP_ADDRESS
# ip_address = "10.170.1.132" #Our IP_ADDRESS
ip_address = "192.168.42.89" #Our IP_ADDRESS
# ip_address = "192.168.13.34" #Our IP_ADDRESS


app = Flask(__name__)

rclpy.init()

class serverPublish(Node):
    def __init__(self):
        super().__init__('server_node')
        self.send_cmd_shape = self.create_publisher(Int32, "/shape_cmd", 1)
        self.send_cmd_twist = self.create_publisher(Point, "/velocity_cmd", 1)
        # self.timer_vel = self.create_timer(0.1, self.twist_CB)
        # self.timer_shape = self.create_timer(0.1, self.shape_CB)

        self.twist_msg = Point()
        self.shape_msg  = Int32()
    def twist_CB(self):
        self.send_cmd_twist.publish(self.twist_msg)

    def shape_CB(self):
        self.send_cmd_shape.publish(self.shape_msg)

    def send_data_shape(self, data):
        # msg = Int32()
        self.shape_msg.data = data
        # msg.data = data
        self.send_cmd_shape.publish(self.shape_msg)
        print("Send_data Shape: %s" % self.shape_msg.data)

    def set_data_vel(self, velocity):
  
        self.twist_msg.x = float(velocity[0])
        self.twist_msg.y = float(velocity[1])
        self.twist_msg.z = 0.0
        self.send_cmd_twist.publish(self.twist_msg)
        print("Send Velocity Command: %.2f | %.2f | %.2f" % (self.twist_msg.x , self.twist_msg.y , self.twist_msg.z))

    def set_data_angular(self, angular):
        self.twist_msg.x = 0.0
        self.twist_msg.y = 0.0
        self.twist_msg.z = float(angular)
        self.send_cmd_twist.publish(self.twist_msg)
        print("Send Angular Command: %.2f | %.2f | %.2f" % (self.twist_msg.x , self.twist_msg.y , self.twist_msg.z))

server_node = serverPublish()

@app.route("/")
def default():
    vel_def = [0,0,0]
    server_node.set_data_vel(vel_def)
    
@app.route("/SHAPE", methods=['POST'])
def shape_received():
    # msg = String()
    data = request.json  # Parses the incoming JSON data
    if(data["SHAPE"] == "I"):
        server_node.send_data_shape(0)
    elif(data["SHAPE"] == "J"):
        server_node.send_data_shape(1)
    elif(data["SHAPE"] == "L"):
        server_node.send_data_shape(2)
    elif(data["SHAPE"] == "O"):
        server_node.send_data_shape(3)
    elif(data["SHAPE"] == "S"):
        server_node.send_data_shape(4)
    elif(data["SHAPE"] == "T"):
        server_node.send_data_shape(5)
    elif(data["SHAPE"] == "Z"):
        server_node.send_data_shape(6)
        
    # ic(request.data)  # Print raw data for debugging
    # ic(data)  # Print parsed JSON data for debugging
    return jsonify({"Received data": data})


@app.route("/SPEED_LINEAR", methods=['POST'])
def linear_speed_received():
    data = request.json  # Parses the incoming JSON data
    server_node.set_data_vel(data["SPEED_LINEAR"])
    # ic(data["SPEED_LINEAR"][0],data["SPEED_LINEAR"][1])
    # ic(data["SPEED_LINEAR"])
    # # test = [1, 2]
    # ic(request.data)  # Print raw data for debugging
    # ic(data)  # Print parsed JSON data for debugging
    return jsonify({"Received data": data})


# @app.route("/STOP", methods=['POST'])
# def stopspeed():
#     data = request.json  # Parses the incoming JSON data
#     server_node.set_data_vel(data["STOP"])
#     # ic(data["SPEED_LINEAR"][0],data["SPEED_LINEAR"][1])
#     # ic(data["SPEED_LINEAR"])
#     # # test = [1, 2]
#     # ic(request.data)  # Print raw data for debugging
#     # ic(data)  # Print parsed JSON data for debugging
#     return jsonify({"Received data": data})


@app.route("/SPEED_ANGULAR", methods=["POST"])
def angular_speed_received():
    data = request.json  # Parses the incoming JSON data
    server_node.set_data_angular(data["SPEED_ANGULAR"])
    # ic(data["SPEED_ANGULAR"])
    ic(request.json)
    ic(request.data)  # Print raw data for debugging
    # ic(data)  # Print parsed JSON data for debugging
    # ic("asep")
    return jsonify({"Received data": data})

def run_server():
    app.run(debug=True,host=ip_address, port=5000 ,threaded=True,use_reloader=False)

def main(args = None):
    print("Start server")
    flask_thread = threading.Thread(target=run_server)
    flask_thread.start()
    try:
        rclpy.spin(server_node)
    except KeyboardInterrupt:
        print("Shutting down ROS 2 node...")
    finally:
        server_node.destroy_node()
        rclpy.shutdown()
        flask_thread.join()  # Wait for Flask thread to finish

if __name__ == '__main__':
    main()
   
