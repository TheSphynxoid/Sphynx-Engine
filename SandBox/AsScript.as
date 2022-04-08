#include "System"

class Movement : Sphynx::Component{
    void Start()
    {
        Print("Hello From AngelScript");
        Print("ID: " + InstanceID);
    }
    void Update()
    {
        Sphynx::Vector3 v = Sphynx::Vector3(transform.Position.x, 
            transform.Position.y, transform.Position.z);
        if(Sphynx::Input::IsKeyPressed(Sphynx::Keys::Up)){
            v.y -=0.01f;
        }if(Sphynx::Input::IsKeyPressed(Sphynx::Keys::Down)){
            v.y+=0.01f;
        }
        transform.MoveTo(v);
    }
    void OnDetach(){

    }
}