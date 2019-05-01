
#include "PhysicsEngine.h"





PhysicsEngine::PhysicsEngine()
{
	multiplier = 1.0f / 60;
}


PhysicsEngine::~PhysicsEngine()
{

}


void PhysicsEngine::updateKeys() {
	//Updates each script to have a boolean array for each of the keys and if they are down.
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
	//Initiates all the scripts.
	window = _window;

	unsigned int id = 0;
	for (Object obj : scene.getObjects()) {
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

	updateKeys();

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
	//Here is where the user could include a loading screen.
}



void PhysicsEngine::update(Scene &scene, std::string PROJpath) {
	update(scene, -1.0f, PROJpath);
}

void PhysicsEngine::update(Scene &scene, float deltaTime, std::string PROJpath) {
	newActorRequest = false;
	unsigned int i = 0;
	for (Object obj : scene.getObjects()) {
		//Will only run the code if the physics body is enabled.
		if (obj.physicsBody.enabled) {
			obj.physicsBody.update(obj.getTransformStruct());

			glm::vec3 RESULTANT_FORCE(0.0f);
			glm::vec3 RESULTANT_ANGULAR(0.0f);

			//Update scripts.
			for (std::string script : obj.componentScriptIDs) {
				//Finds the script references in the object, from the physics scripts vector.
				if (scripts.find(script) != scripts.end()) {
					if (obj.componentScriptData.find(script) != obj.componentScriptData.end()) {
						//Checks if the script is enabled in the object.
						if (obj.componentScriptData.at(script).enabled) {
							try {
								//Transfers object data and obj specific variables to script, and then updates it.
								obj.raycasts.clear();
								obj.OnFixedUpdate(scripts.at(script), script, window, deltaTime);

							}
							catch (std::exception ex) {
								std::cout << "could not update script" << std::endl;
							}

							try {
								//For any raycast calls from the script, it runs the raycast function.
								for (RayHit rc : obj.raycasts) {
									Collision c = RayCast(scene, obj, rc.length, rc.increments, rc.origin, rc.direction);
									//^^ Returns a Hit structure for if the raycast intercepted any collision boxes.
									if (c.collided) {
										//If the collision occured, then the script's OnRaycast overridable function is called.
										rc.collision = c;
										obj.OnRaycast(rc, scripts[script], script, window);
										break;
									}
								}

							}
							catch (std::exception ex) {
								std::cout << "could not run script raycasting." << std::endl;
							}

							//Checks if the script calls for a new scene or GUI.
							if (obj.newUI != "") {
								newUI = obj.newUI;
								scripts[script]->resetNewUI();
							}
							if (obj.newScene != "") {
								scripts[script]->resetNewScene();
								ChangeScene(scene, PROJpath + "Scenes\\" + obj.newScene + ".SCENE");
								obj.newScene = "";
								return;
							}

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


				for (BoxCollider objBox : obj.physicsBody.coll_Box) {
					unsigned int index = 0;
					std::vector<Collision> collisions;
					collisions.clear();
					objBox.translate(obj.getTransformStruct(), objBox); //Translate bounds according to object.

																		//Detecting collisions.
					for (Object colliderObj : scene.getObjects()) {
						if (index != i && obj.distanceTo(colliderObj.pos) < 20.0f) { //Must be in a range of 20 m for it to check collisions.
																					 //Checks if the objects are the same in the loop.
							for (BoxCollider collider : colliderObj.physicsBody.coll_Box) {

								collider.translate(colliderObj.getTransformStruct(), collider); //Translate bounds according to object.

								float pitch = colliderObj.getTransformStruct().pitch;
								float yaw = colliderObj.getTransformStruct().yaw;
								float roll = colliderObj.getTransformStruct().roll;
								Collision c;
								c = checkBoxCollision(objBox, collider);


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
					for (Collision c : collisions) {
						Object* hit_obj = &scene.objects[c.objID];
						for (std::string scriptID : hit_obj->componentScriptIDs) {
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
			glm::vec3 GRAVITY = { 0.0f, -100.0, 0.0f };
			if (obj.physicsBody.doesGravity) {
				RESULTANT_FORCE += GRAVITY;
			}
			for (glm::vec3 force : obj.forces) {
				RESULTANT_FORCE += force;
			}
			obj.forces.clear();

			RESULTANT_FORCE /= obj.physicsBody.mass; //Calculating acceleration.
			RESULTANT_FORCE *= deltaTime; //modifying it to fit with time frame.

			obj.pos += deltaTime * obj.physicsBody.linear_velocity; //Updating object position.
			obj.physicsBody.linear_velocity += RESULTANT_FORCE; //Velocity is increased due to acceleration.


																//Rotation
			//Similar process to forces, just changes rotation.
			for (glm::vec3 torque : obj.torques) {
				RESULTANT_ANGULAR += torque;
			}
			obj.torques.clear();

			RESULTANT_ANGULAR *= deltaTime;
			obj.roll += deltaTime * obj.physicsBody.angular_velocity.x;
			obj.pitch += deltaTime * obj.physicsBody.angular_velocity.y;
			obj.yaw += deltaTime * obj.physicsBody.angular_velocity.z;
			obj.physicsBody.angular_velocity += RESULTANT_ANGULAR;





			//Updating scene with new object data.
			if (obj.deleteSelf) {
				scene.deleteObject(i);
				i -= 1; //1 Less item in the array.
			}
			else {

				if (obj.newActors.size() > 0) {
					newActorRequest = true;
				}
				scene.setObject(i, obj);
			}
		}
			
		
		i++;
	}

	updateKeys();
}

void PhysicsEngine::load(std::string name, Scene &scene) {
	scene.loadBinary(name);
}

std::string PhysicsEngine::addScript(ComponentScript* script) {
	//Adds a script pointer to the vector of scripts.
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
	for (Object box : scene.getObjects()) {
		//Checks if the object and box are the same.
		if (box.pos != obj.pos) {
			if (box.physicsBody.collides) {
				Collision col;
				//Will only work if it wants to be collided.

					for (BoxCollider boxc : box.physicsBody.coll_Box) {
						//Translates the box collider to the object's position and scale etc.
						boxc.translate(box.getTransformStruct(), boxc);
						if (obj.distanceTo(boxc.position) <= ray.distance) {
							//Calculates the distance between the ray and box. Must be less than the maximum length of the raycast.
							col = checkLineCollision(ray, boxc);
							//Checks if a collision occurs.
							if (col.collided) {
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
	float distanceTo = 1000000.0f;
	//Retrieving the closest collision to the raycast's origin.
	for (Collision col : collisions) {
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
	//Iterates through a fixed step between the origin and the direction / end point.
	for (float d = 0; d <= line.distance * 100; d += line.increments * 100) {
		currentPos = line.origin + (line.direction * (float)(d / 100));
		//Checks if the tip / vertex of the raycast line is within the box collision mesh.
		if (currentPos.x <= box.maxX && currentPos.x >= box.minX) {
			if (currentPos.y <= box.maxY && currentPos.y >= box.minY) {
				if (currentPos.z <= box.maxZ && currentPos.z >= box.minZ) {
					//Calculates the distance intercepted.
					c.collided = true;
					c.distanceX = box.maxX - currentPos.x;
					c.distanceY = box.maxY - currentPos.y;
					c.distanceZ = box.maxZ - currentPos.z;
					//Records the impact position.
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

	//Checks if any of the vertices in one collision mesh are within the mesh of the other collision box.
	for (glm::vec3 vertex : box1.corners) {
		if (vertex.x <= box2.maxX && vertex.x >= box2.minX) {
			if (vertex.y <= box2.maxY && vertex.y >= box2.minY) {
				if (vertex.z <= box2.maxZ && vertex.z >= box2.minZ) {
					Collision c;
					c.collided = true;
					c.distanceX = 0.0f;
					c.distanceZ = 0.0f;
					c.distanceY = 0.0f;
					//Calculates the distance to the closest face.
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