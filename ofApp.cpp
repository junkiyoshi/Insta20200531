#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	
	auto ico_sphere = ofIcoSpherePrimitive(250, 4);
	this->triangle_list = ico_sphere.getMesh().getUniqueFaces();

	for (auto& triangle : this->triangle_list) {

		glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		this->location_list.push_back(avg);
		this->param_list.push_back(0);
	}

	int span = 13;
	for (auto& location : this->location_list) {

		vector<int> route_info = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				route_info.push_back(index);
			}
		}

		this->route_info_list.push_back(route_info);
	}

	for (int i = 0; i < 150; i++) {

		this->index_list.push_back((int)ofRandom(this->location_list.size()));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	for (int i = 0; i < this->index_list.size(); i++) {

		int next_index = this->route_info_list[this->index_list[i]][(int)ofRandom(this->route_info_list[this->index_list[i]].size())];
		for (int k = 0; k < this->route_info_list[this->index_list[i]].size(); k++) {

			if (this->param_list[next_index] <= 0) {

				this->param_list[next_index] = 30;
				this->index_list[i] = next_index;
				break;
			}
		}
	}

	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->param_list[i] > 0) {

			int index = this->mesh.getNumVertices();

			this->mesh.addVertex(this->triangle_list[i].getVertex(0));
			this->mesh.addVertex(this->triangle_list[i].getVertex(1));
			this->mesh.addVertex(this->triangle_list[i].getVertex(2));

			this->mesh.addTriangle(index + 0, index + 1, index + 2);
		}

		if (this->param_list[i] > 0) { this->param_list[i] -= 1; }
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);

	ofSetColor(239);
	this->mesh.drawWireframe();

	ofSetColor(39);
	this->mesh.drawFaces();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}