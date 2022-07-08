class Camera : Sphynx::Component{
    Sphynx::Internal::Camera@ NativeCamera;
    float FOV{
            get{
                return GetFOV();
            }
            set{
                SetFOV(value);
            }
    }
    void Update()
    {
    }
    void OnDetach(){
    }
}