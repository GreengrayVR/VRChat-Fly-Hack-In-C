#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

/*
	// RVA: 0x24E6030 Offset: 0x24E5030 VA: 0x1824E6030
	public static VRCPlayerApi get_LocalPlayer() { }

    // RVA: 0x853110 Offset: 0x852110 VA: 0x180853110
    internal static VRC.Player get_VRCLocalPlayer() { }

    // RVA: 0x2EC2A00 Offset: 0x2EC1A00 VA: 0x182EC2A00
    public Transform get_transform() { }

    // RVA: 0x3163DD0 Offset: 0x3162DD0 VA: 0x183163DD0
    public void set_position(Vector3 value) { }

    // RVA: 0x2EBDD30 Offset: 0x2EBCD30 VA: 0x182EBDD30
    public static Camera get_main() { }

    // RVA: 0x3163650 Offset: 0x3162650 VA: 0x183163650
    public Vector3 get_up() { }

    // RVA: 0x3162DF0 Offset: 0x3161DF0 VA: 0x183162DF0
    public Vector3 get_forward() { }

    // RVA: 0x3163460 Offset: 0x3162460 VA: 0x183163460
    public Vector3 get_right() { }

    // RVA: 0x3163410 Offset: 0x3162410 VA: 0x183163410
    public Vector3 get_position() { }

    // RVA: 0x2D521D0 Offset: 0x2D511D0 VA: 0x182D521D0
    public static Type GetType(string typeName) { }

    // RVA: 0x2B380A0 Offset: 0x2B370A0 VA: 0x182B380A0
    public Component GetComponent(Type type) { }

    // RVA: 0x2EC2970 Offset: 0x2EC1970 VA: 0x182EC2970
    public GameObject get_gameObject() { }

    // RVA: 0x467DDA0 Offset: 0x467CDA0 VA: 0x18467DDA0
    public void set_enabled(bool value) { }
*/

typedef struct Vector3
{
    float x, y, z;
} Vector3;

HMODULE GameAssemblyHandle;
HMODULE HModule;

typedef void* (*VRCPlayerApi_LocalPlayer_t)();
typedef void* (*Player_LocalPlayer_t)();
typedef void* (*Component_GetTransform_t)(void*);
typedef void (*Transform_SetPosition_t)(void*, Vector3);
typedef void* (*Camera_Main_t)();
typedef Vector3 (*Transform_Up_t)(void*);
typedef Vector3 (*Transform_Forward_t)(void*);
typedef Vector3 (*Transform_Right_t)(void*);
typedef Vector3 (*Transform_GetPosition_t)(void*);
typedef Vector3 (*Transform_GetPosition_t)(void*);
typedef void* (*Type_GetType_t)(void*);
typedef void* (*GameObject_GetComponent_t)(void*, void*);
typedef void* (*Component_GetGameObject_t)(void*);
typedef void (*Collider_SetEnabled_t)(void*, int);

typedef void* (*IL2CPP_String_New_t)(const char*);

VRCPlayerApi_LocalPlayer_t VRCPlayerApi_LocalPlayer;
Player_LocalPlayer_t Player_LocalPlayer;
Component_GetTransform_t Component_GetTransform;
Transform_SetPosition_t Transform_SetPosition;
Camera_Main_t Camera_Main;
Transform_Up_t Transform_Up;
Transform_Forward_t Transform_Forward;
Transform_Right_t Transform_Right;
Transform_GetPosition_t Transform_GetPosition;
Type_GetType_t Type_GetType;
GameObject_GetComponent_t GameObject_GetComponent;
Component_GetGameObject_t Component_GetGameObject;
Collider_SetEnabled_t Collider_SetEnabled;

IL2CPP_String_New_t IL2CPP_String_New;

void init_functions()
{
    GameAssemblyHandle = GetModuleHandleW(L"GameAssembly.dll");

    VRCPlayerApi_LocalPlayer = (VRCPlayerApi_LocalPlayer_t)((uint8_t*)GameAssemblyHandle + 0x24E6030);
    Player_LocalPlayer = (Player_LocalPlayer_t)((uint8_t*)GameAssemblyHandle + 0x853110);
    Component_GetTransform = (Component_GetTransform_t)((uint8_t*)GameAssemblyHandle + 0x2EC2A00);
    Transform_SetPosition = (Component_GetTransform_t)((uint8_t*)GameAssemblyHandle + 0x3163DD0);
    Camera_Main = (Camera_Main_t)((uint8_t*)GameAssemblyHandle + 0x2EBDD30);
    Transform_Up = (Transform_Up_t)((uint8_t*)GameAssemblyHandle + 0x3163650);
    Transform_Forward = (Transform_Forward_t)((uint8_t*)GameAssemblyHandle + 0x3162DF0);
    Transform_Right = (Transform_Right_t)((uint8_t*)GameAssemblyHandle + 0x3163460);
    Transform_GetPosition = (Transform_GetPosition_t)((uint8_t*)GameAssemblyHandle + 0x3163410);
    Type_GetType = (Type_GetType_t)((uint8_t*)GameAssemblyHandle + 0x2D521D0);
    GameObject_GetComponent = (GameObject_GetComponent_t)((uint8_t*)GameAssemblyHandle + 0x2B380A0);
    Component_GetGameObject = (Component_GetGameObject_t)((uint8_t*)GameAssemblyHandle + 0x2EC2970);
    Collider_SetEnabled = (Collider_SetEnabled_t)((uint8_t*)GameAssemblyHandle + 0x467DDA0);

    IL2CPP_String_New = (IL2CPP_String_New_t)GetProcAddress(GameAssemblyHandle, "il2cpp_string_new");
}


uint8_t fly_toggle = 0;


#define F_KEY 0x46

#define Q_KEY 0x51
#define E_KEY 0x45

#define W_KEY 0x57
#define A_KEY 0x41
#define S_KEY 0x53
#define D_KEY 0x44

#define DELETE_KEY 0x2E

void update()
{
    void* player = Player_LocalPlayer();

    if (player == NULL)
        return;

    if (GetAsyncKeyState(F_KEY) & 1)
    {
        fly_toggle = !fly_toggle;

        void* type = Type_GetType(IL2CPP_String_New("UnityEngine.CharacterController, UnityEngine.PhysicsModule"));

        void* gameobject = Component_GetGameObject(player);

        void* component = GameObject_GetComponent(gameobject, type);

        Collider_SetEnabled(component, !fly_toggle);

        printf("Fly is %d\n", fly_toggle);
    }


    if (!fly_toggle)
        return;

    void* transform = Component_GetTransform(player);
    void* camera_transform = Component_GetTransform(Camera_Main());

    if (GetAsyncKeyState(W_KEY))
    {
        Vector3 camera_forward = Transform_Forward(camera_transform);
        Vector3 pos = Transform_GetPosition(transform);
        pos.x += camera_forward.x * (0.5f / 3000.0f);
        pos.y += camera_forward.y * (0.5f / 3000.0f);
        pos.z += camera_forward.z * (0.5f / 3000.0f);

        Transform_SetPosition(transform, pos);
    }

    if (GetAsyncKeyState(A_KEY))
    {
        Vector3 camera_right = Transform_Right(camera_transform);
        Vector3 pos = Transform_GetPosition(transform);
        pos.x -= camera_right.x * (0.5f / 3000.0f);
        pos.y -= camera_right.y * (0.5f / 3000.0f);
        pos.z -= camera_right.z * (0.5f / 3000.0f);

        Transform_SetPosition(transform, pos);
    }

    if (GetAsyncKeyState(S_KEY))
    {
        Vector3 camera_forward = Transform_Forward(camera_transform);
        Vector3 pos = Transform_GetPosition(transform);
        pos.x -= camera_forward.x * (0.5f / 3000.0f);
        pos.y -= camera_forward.y * (0.5f / 3000.0f);
        pos.z -= camera_forward.z * (0.5f / 3000.0f);

        Transform_SetPosition(transform, pos);
    }

    if (GetAsyncKeyState(D_KEY))
    {
        Vector3 camera_right = Transform_Right(camera_transform);
        Vector3 pos = Transform_GetPosition(transform);
        pos.x += camera_right.x * (0.5f / 3000.0f);
        pos.y += camera_right.y * (0.5f / 3000.0f);
        pos.z += camera_right.z * (0.5f / 3000.0f);

        Transform_SetPosition(transform, pos);
    }

    if (GetAsyncKeyState(Q_KEY))
    {
        Vector3 camera_up = Transform_Up(camera_transform);
        Vector3 pos = Transform_GetPosition(transform);
        pos.x -= camera_up.x * (0.5f / 3000.0f);
        pos.y -= camera_up.y * (0.5f / 3000.0f);
        pos.z -= camera_up.z * (0.5f / 3000.0f);

        Transform_SetPosition(transform, pos);
    }

    if (GetAsyncKeyState(E_KEY))
    {
        Vector3 camera_right = Transform_Up(camera_transform);
        Vector3 pos = Transform_GetPosition(transform);
        pos.x += camera_right.x * (0.5f / 3000.0f);
        pos.y += camera_right.y * (0.5f / 3000.0f);
        pos.z += camera_right.z * (0.5f / 3000.0f);

        Transform_SetPosition(transform, pos);
    }
}

void run()
{
    init_functions();

    while (1)
    {
        if (GetAsyncKeyState(DELETE_KEY) & 1)
            break;

        if (VRCPlayerApi_LocalPlayer() == NULL)
            continue;


        update();
    }

    FreeLibraryAndExitThread(HModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HModule = hModule;
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)run, 0, 0, 0);
        break;
    }
    
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}