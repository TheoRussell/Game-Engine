
#include "PhysicsEngine.h"





PhysicsEngine::PhysicsEngine()
{
	multiplier = 1.0f / 60;
}


PhysicsEngine::~PhysicsEngine()
{

}


void PhysicsEngine::updateKeys() {
	for (std::pair<std::string, ComponentScript*> cs : scripts) {
		try {
			cs.second->setScriptWindow(window);
			cs.second->updateKeys();
		}
		catch (std::exception ex) {
			std::cout << ex.what() << std::endl;
		}
	}
}

void PhysicsEngine::start(Scene &scene, GLFWwindow * _window) {
	window = _window;

	unsigned int id = 0;
	for each (Object obj in scene.getObjects()) {
		for (std::string script : obj.componentScriptIDs) {
			try {
				obj.startScript(scripts.at(script), script, window);
			}
			catch (std::exception ex) {
				std::cout << "Could not bind script" << std::endl;
			}

		}
		scene.setObject(id, obj);
		id++;
		
	}
}

void PhysicsEngine::OnStart(Scene &scene, GLFWwindow * _window) {
	window = _window;

	//updateKeys();

	unsigned int id = 0;
	for each (Object obj in scene.getObjects()) {
		for (std::string script : obj.componentScriptIDs) {
				try {
					obj.OnStart(scripts.at(script), script, window);
				}
				catch (std::exception ex) {
					std::cout << "Could not start script" << std::endl;
				}

			}
			scene.setObject(id, obj);
			id++;
	}
}


void PhysicsEngine::ChangeScene(Scene &scene, std::string path) {
	scene.loadBinary(path);
	//Here is where the user could include a loading screen if it wasn't so speedy.
}



void PhysicsEngine::update(Scene &scene, std::string PROJpath) {
	update(scene, -1.0f, PROJpath);
}

void PhysicsEngine::update(Scene &scene, float deltaTime, std::string PROJpath) {
	unsigned int i = 0;
	for each (Object obj in scene.getObjects()) {
			obj.physicsBody.update(obj.getTransformStruct());

			glm::vec3 RESULTANT_FORCE(0.0f);
			glm::vec3 RESULTANT_ANGULAR(0.0f);

			//Impulse imp;
			//imp.time = 5.0f;
			//imp.force = glm::vec3(0.0f, 10.0f, 1000.0f);
			//imp.time -= deltaTime;
			//RESULTANT_FORCE += imp.force;

			//Update scripts.
			for (std::string script : obj.componentScriptIDs) {
				if (scripts.find(script) != scripts.end()) {
					if (obj.componentScriptData.at(script).enabled) {
						try {
							obj.raycasts.clear();
							if (deltaTime == -1) {
								obj.OnUpdate(scripts.at(script), script, window);
							}
							else {
								obj.OnFixedUpdate(scripts.at(script), script, window, deltaTime);
							}

							
						}
						catch (std::exception ex) {
							std::cout << "could not update script" << std::endl;
						}

						try {
							for each (RayHit rc in obj.raycasts) {
								Collision c = RayCast(scene, obj, rc.length, rc.increments, rc.origin, rc.direction);
								if (c.collided) {
									rc.collision = c;
									obj.OnRaycast(rc, scripts[script], script, window);
									break;
								}
							}
							
						}
						catch (std::exception ex) {
							std::cout << "could not run script raycasting." << std::endl;
						}


						if (obj.newScene != "") {
							scripts[script]->resetNewScene();
							ChangeScene(scene, PROJpath + "Scenes\\" + obj.newScene + ".SCENE");
							obj.newScene = "";
							return;
						}
						if (obj.newUI != "") {
							newUI = obj.newUI;
							scripts[script]->resetNewUI();
						}
					}

				}
			}

			if (obj.exitGame) {
				exitGame = true;
				return;
			}


			//Collisions.			
			bool collides = false;
			if (obj.physicsBody.collides) {

				if (obj.physicsBody.doesGravity) {
					obj.pos -= glm::vec3(0.0f, 9.81f * multiplier, 0.0f);
				}


				for each (BoxCollider objBox in obj.physicsBody.coll_Box) {
					unsigned int index = 0;
					std::vector<Collision> collisions;
					collisions.clear();
					objBox.translate(obj.getTransformStruct(), objBox); //Translate bounds according to object.

					//Detecting collisions.
					for each (Object colliderObj in scene.getObjects()) {
							if (index != i && obj.distanceTo(colliderObj.pos) < 20.0f) { //Must be in a range of 20 m for it to check collisions.
																						 //Checks if the objects are the same in the loop.
								for each (BoxCollider collider in colliderObj.physicsBody.coll_Box) {

									collider.translate(colliderObj.getTransformStruct(), collider); //Translate bounds according to object.

									float pitch = colliderObj.getTransformStruct().pitch;
									float yaw = colliderObj.getTransformStruct().yaw;
									float roll = colliderObj.getTransformStruct().roll;
									Collision c;
									if ((int)pitch % 360 > 0.0f || (int)pitch % 360 < 0.0f || (int)yaw % 360 > 0.0f || (int)yaw % 360 < 0.0f || (int)roll % 360 > 0.0f || (int)roll % 360 < 0.0f) {
										c = checkBoxCollision(objBox, collider, roll, pitch, yaw);
									}
									else {
										c = checkBoxCollision(objBox, collider);
									}


									if (c.collided) {
										c.hit_label = colliderObj.name;
										c.hit_mass = colliderObj.physicsBody.mass;
										c.hit_linear_velocity = colliderObj.physicsBody.linear_velocity;
										c.hit_angular_velocity = colliderObj.physicsBody.angular_velocity;

										c.b2_max = glm::vec3(collider.maxX, collider.maxY, collider.maxZ);
										c.b2_min = glm::vec3(collider.minX, collider.minY, collider.minZ);
										c.b1_max = glm::vec3(objBox.maxX, objBox.maxY, objBox.maxZ);
										c.b1_min = glm::vec3(objBox.minX, objBox.minY, objBox.minZ);

										c.objID = index;
										collisions.push_back(c);
										//The current object collides with an object on the scene...
										collides = true;
									}
								}
							}
						
						index++;
					}
					//Running collision scripts.
					for each (Collision c in collisions) {
						Object* hit_obj = &scene.objects[c.objID];
						for (std::string scriptID: hit_obj->componentScriptIDs) {
							if (scripts.find(scriptID) != scripts.end()) {
								try {
									hit_obj->OnCollide(c, scripts.at(scriptID), scriptID, window);
								}
								catch (std::exception ex) {
									std::cout << "Could not run collision script." << std::endl;
								}
							}
						}

						//Affect object after all collisions are detected.
						for (std::string scriptID : obj.componentScriptIDs) {
							if (scripts.find(scriptID) != scripts.end()) {
								if (obj.componentScriptData.at(scriptID).enabled) {
									try {
										obj.OnCollide(c, scripts.at(scriptID), scriptID, window);
									}
									catch (std::exception ex) {
										std::cout << "could not collide script" << std::endl;
									}

									try {
										for each (RayHit rc in obj.raycasts) {
											Collision c = RayCast(scene, obj, rc.length, rc.increments, rc.origin, rc.direction);
											if (c.collided) {
												rc.collision = c;
												obj.OnRaycast(rc, scripts.at(scriptID), scriptID, window);
												break;
											}
										}
										obj.raycasts.clear();
									}
									catch (std::exception ex) {
										std::cout << "could not run script raycasting." << std::endl;
									}

									if (obj.newScene != "") {
										scripts.at(scriptID)->resetNewScene();
										obj.newScene = "";
										ChangeScene(scene, PROJpath + "\\Scenes\\" + obj.newScene + ".SCENE");
										return;
									}

									if (obj.newUI != "") {
										newUI = obj.newUI;
										scripts.at(scriptID)->resetNewUI();
									}
								}
							}
						}
					}
				}
			}

			//Do even if object is selected.
			
			///Forces
			//Done in metres per second.
			//Translation.
			glm::vec3 GRAVITY = {0.0f, -100.0, 0.0f};
			if (obj.physicsBody.doesGravity) {
				RESULTANT_FORCE += GRAVITY;
			}
			for each (glm::vec3 force in obj.forces) {
				RESULTANT_FORCE += force;
			}
			obj.forces.clear();

			RESULTANT_FORCE /= obj.physicsBody.mass; //Calculating acceleration.
			RESULTANT_FORCE *= deltaTime; //modifying it to fit with time frame.

			obj.pos += deltaTime * obj.physicsBody.linear_velocity; //Updating object position.
			obj.physicsBody.linear_velocity += RESULTANT_FORCE; //Velocity is increased due to acceleration.


			//Rotation
			for each (glm::vec3 torque in obj.torques) {
				RESULTANT_ANGULAR += torque;
			}
			obj.torques.clear();

			RESULTANT_ANGULAR *= deltaTime;
			obj.pitch += deltaTime * obj.physicsBody.angular_velocity.x;
			obj.roll += deltaTime * obj.physicsBody.angular_velocity.y;
			obj.yaw += deltaTime * obj.physicsBody.angular_velocity.z;
			obj.physicsBody.angular_velocity += RESULTANT_ANGULAR;





			//Updating scene with new object data.
			if (obj.deleteSelf) {
				scene.deleteObject(i);
				i -= 1; //1 Less item in the array.
			}
			else {
				scene.setObject(i, obj);
			}
		
		i++;
	}

	updateKeys();
}

void PhysicsEngine::load(std::string name, Scene &scene) {
	scene.loadBinary(name);
}

std::string PhysicsEngine::addScript(ComponentScript* script) {
	scripts.insert(std::pair<std::string, ComponentScript*>(script->GetName(),script));
	return script->GetName();
}

Collision PhysicsEngine::RayCast(Scene &scene, Object obj) {
	return RayCast(scene, obj, 6.0f, 0.1f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

Collision PhysicsEngine::RayCast(Scene &scene, Object obj, float distance, float increments, glm::vec3 origin, glm::vec3 direction) {
	obj.physicsBody.update(obj.getTransformStruct());
	LineCollider ray;
	ray.direction = direction;
	ray.origin = origin;
	ray.distance = distance;
	ray.increments = increments;
	unsigned int id = 0;
	std::vector<Collision> collisions;
	for each (Object box in scene.getObjects()) {
		if (box.pos != obj.pos) {
			if (box.physicsBody.collides) {
				Collision col;
				//Will only work if it wants to be collided.

					for each (BoxCollider boxc in box.physicsBody.coll_Box) {
						boxc.translate(box.getTransformStruct(), boxc);
						if (obj.distanceTo(boxc.position) <= ray.distance) {
							col = checkLineCollision(ray, boxc);
							if (col.collided) {
								std::cout << col.impact_position.x << "," << col.impact_position.y << "," << col.impact_position.z << std::endl;
								col.objID = id;
								collisions.push_back(col);
								break; //Only detect one physics collision per object.
							}
						}

					}
			}
		}
		
		id++;
	}

	Collision c;
	float distanceTo = 10000.0f;
	for each (Collision col in collisions) {
		//Gets the closest collision.
		float dist = obj.distanceTo(scene.getObject(col.objID).pos);
		if (dist < distanceTo) {
			distanceTo = dist;
			c = col;
		}
	}
	return c;
}

Collision PhysicsEngine::checkLineCollision(LineCollider &line, BoxCollider &box) {
	glm::vec3 currentPos;
	Collision c;
	for (float d = 0; d <= line.distance * 100; d += line.increments * 100) {
		currentPos = line.origin + (line.direction * (float)(d / 100));
		if (currentPos.x <= box.maxX && currentPos.x >= box.minX) {
			if (currentPos.y <= box.maxY && currentPos.y >= box.minY) {
				if (currentPos.z <= box.maxZ && currentPos.z >= box.minZ) {
					c.collided = true;
					c.distanceX = box.maxX - currentPos.x;
					c.distanceY = box.maxY - currentPos.y;
					c.distanceZ = box.maxZ - currentPos.z;
					c.impact_position = currentPos;
					return c;
				}
			}
		}
	}
	c.collided = false;
	return c;
}

Collision PhysicsEngine::checkBoxCollision(BoxCollider &box1, BoxCollider &box2) {


	for each (glm::vec3 vertex in box1.corners) {
		if (vertex.x <= box2.maxX && vertex.x >= box2.minX) {
			if (vertex.y <= box2.maxY && vertex.y >= box2.minY) {
				if (vertex.z <= box2.maxZ && vertex.z >= box2.minZ) {
					Collision c;
					c.collided = true;
					c.distanceX = 0.0f;
					c.distanceZ = 0.0f;
					c.distanceY = 0.0f;

					if ((box2.maxX - vertex.x) >= (vertex.x - box2.minX)) {
						c.distanceX = box2.minX - vertex.x;
						//Lower x bound of box.
					}
					else {
						c.distanceX = box2.maxX - vertex.x;
					}

					if ((box2.maxZ - vertex.z) >= (vertex.z - box2.minZ)) {
						//Lower z bound of box.{
						c.distanceZ = box2.minZ - vertex.z;

					}
					else {
						c.distanceZ = box2.maxZ - vertex.z;
					}


					if ((box2.maxY - vertex.y) <= (vertex.y - box2.minY)) {
						c.distanceY = box2.maxY - vertex.y;
					}
					else {
						c.distanceY = -(vertex.y - box2.minY);
					}
					return c;
				}
			}
		}
	}
	Collision c;
	c.collided = false;
	return c;
}


glm::vec3 PhysicsEngine::findMid(std::vector<glm::vec3> &corners) {
	float lowest = 9999999.9f;
	float highest = -999999.9f;
	std::vector<glm::vec3> topCorners;
	std::vector<glm::vec3> bottomCorners;
	for each (glm::vec3 vertex in corners) {
		if (vertex.y < lowest) {
			lowest = vertex.y;
			bottomCorners.clear();
			bottomCorners.push_back(vertex);
		}
		else if (vertex.y == lowest) {
			bottomCorners.push_back(vertex);
		}
		if (vertex.y > highest) {
			highest = vertex.y;
			topCorners.clear();
			topCorners.push_back(vertex);
		}
		else if (vertex.y == highest) {
			topCorners.push_back(vertex);
		}
	}
	glm::vec3 top = getBiggestXZ(topCorners);
	glm::vec3 low = getLowestXZ(bottomCorners);
	return  glm::vec3((top.x + low.x) / 2, (top.y + low.y) / 2, (top.z + low.z) / 2);
}

glm::vec3 PhysicsEngine::getBiggestXZ(std::vector<glm::vec3> &corners) {
	float maxX = -99999.9f;
	std::vector<glm::vec3> xCorners;
	for each (glm::vec3 vertex in corners) {
		if (vertex.x > maxX) {
			maxX = vertex.x;
			xCorners.clear();
			xCorners.push_back(vertex);
		}
		else if (vertex.x == maxX) {
			xCorners.push_back(vertex);
		}
	}
	float maxZ = -99999.9f;
	std::vector<glm::vec3> zCorners;
	for each (glm::vec3 vertex in corners) {
		if (vertex.z > maxZ) {
			maxZ = vertex.z;
			zCorners.clear();
			zCorners.push_back(vertex);
		}
		else if (vertex.z == maxZ) {
			zCorners.push_back(vertex);
		}
	}
	for each (glm::vec3 vertex in xCorners) {
		for each (glm::vec3 vertex_z in zCorners) {
			if (vertex == vertex_z) {
				return vertex;
			}
		}
	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 PhysicsEngine::getLowestXZ(std::vector<glm::vec3> &corners) {
	float maxX = 99999.9f;
	std::vector<glm::vec3> xCorners;
	for each (glm::vec3 vertex in corners) {
		if (vertex.x < maxX) {
			maxX = vertex.x;
			xCorners.clear();
			xCorners.push_back(vertex);
		}
		else if (vertex.x == maxX) {
			xCorners.push_back(vertex);
		}
	}
	float maxZ = 99999.9f;
	std::vector<glm::vec3> zCorners;
	for each (glm::vec3 vertex in corners) {
		if (vertex.z < maxZ) {
			maxZ = vertex.z;
			zCorners.clear();
			zCorners.push_back(vertex);
		}
		else if (vertex.z == maxZ) {
			zCorners.push_back(vertex);
		}
	}
	for each (glm::vec3 vertex in xCorners) {
		for each (glm::vec3 vertex_z in zCorners) {
			if (vertex == vertex_z) {
				return vertex;
			}
		}
	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

Collision PhysicsEngine::checkBoxCollision(BoxCollider &box1, BoxCollider &box2, float roll, float pitch, float yaw) {
	//roll = x, pitch = y, yaw = z

	glm::vec3 middle = findMid(box2.corners);
	float centreX = middle.x;
	float centreY = middle.y;
	float centreZ = middle.z;

	float yChangeMin = 0.0f;
	float xChangeMin = 0.0f;
	float zChangeMin = 0.0f;
	float yChangeMax = 0.0f;
	float xChangeMax = 0.0f;
	float zChangeMax = 0.0f;
	roll = (float)((int)roll % 360);
	pitch = (float)((int)pitch % 360);
	yaw = (float)((int)yaw % 360);
	if (roll < 0) {
		roll = 360 + roll;
	}
	if (pitch < 0) {
		pitch = 360 + pitch;
	}
	if (yaw < 0) {
		yaw = 360 + yaw;
	}
	roll = (float)((int)roll % 90);
	pitch = (float)((int)pitch % 90);
	yaw = (float)((int)yaw % 90);

	while (roll > 45) {
		roll = 90 - roll;
	}


	std::cout << roll << std::endl;


	for each (glm::vec3 vertex in box1.corners) {
		float yDiff = vertex.y - centreY;
		float xDiff = vertex.x - centreX;
		float zDiff = vertex.z - centreZ;
		
		//std::cout << "Z: " << zDiff  << std::endl;

		yChangeMin = tan(glm::radians(roll)) * zDiff;
		//yChangeMin += tan(glm::radians(-yaw)) * xDiff;




		yChangeMax = tan(glm::radians(roll)) * zDiff;
		//yChangeMax += tan(glm::radians(yaw)) * xDiff;

		yChangeMin += box2.minY;
		xChangeMin += centreX;
		zChangeMin += centreZ;
		yChangeMax += box2.maxY;
		xChangeMax += centreX;
		zChangeMax += centreZ;
		float temp = 0.0f;
		if (xChangeMax < xChangeMin) {
			temp = xChangeMin;
			xChangeMin = xChangeMax;
			xChangeMax = temp;
		}
		if (zChangeMax < zChangeMin) {
			temp = zChangeMin;
			zChangeMin = zChangeMax;
			zChangeMax = temp;
		}

		//if (vertex.x <= box2.maxX && vertex.x >= box2.minX) {
			if (vertex.y <= yChangeMax && vertex.y >= yChangeMin) {
				//if (vertex.z <= box2.maxZ && vertex.z >= box2.minZ) {
					Collision c;
					c.collided = true;
					c.distanceX = box2.maxX - vertex.x;

					if ((yChangeMax - vertex.y) <= (vertex.y - yChangeMin)) {
						c.distanceY = yChangeMax - vertex.y;
					}
					else {
						c.distanceY = - (vertex.y - yChangeMin);
					}



					c.distanceZ = box2.maxZ - vertex.z;
					return c;
				//}
			}
		//}


	}
	Collision c;
	c.collided = false;
	return c;
}