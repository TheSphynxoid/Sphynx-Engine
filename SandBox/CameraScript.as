class Camera : Sphynx::Component{
    Camera(){

    }
    void Start()
    {
        Print("Hello From Camera");
    }
    void Update()
    {
        if(Sphynx::Input::IsKeyPressed(Sphynx::Keys::Up)){
            Print("Up");
        }
    }
    void OnDetach(){

    }
}