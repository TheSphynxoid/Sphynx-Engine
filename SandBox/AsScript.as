
class Movement : Sphynx::Component{
    string VecString(const Sphynx::Vector3 v){
        return "{" + v.x+","+v.y+","+v.z+"}";
    }
    void Start()
    {
        Print("Hello From AngelScript");
        Print("ID: " + InstanceID);
        Print(VecString(Sphynx::Vec3Up));
    }
    void Update()
    {
        Sphynx::Vector3 v = Sphynx::Vector3(transform.Position.x, 
            transform.Position.y, transform.Position.z);
        if(Sphynx::Input::KeyPressed(Sphynx::Keys::Up)){
            v.y +=0.01f;
        }if(Sphynx::Input::KeyPressed(Sphynx::Keys::Down)){
            v.y -=0.01f;
        }
        if(Sphynx::Input::KeyPressed(Sphynx::Keys::Enter)){
            gameObject.Destroy();
        }
        if(Sphynx::Input::KeyPressed(Sphynx::Keys::Left)){
            transform.Rotate(1.0f, Sphynx::Vec3Up);
        }
        transform.MoveTo(v);
    }
    void OnDetach(){

    }
}