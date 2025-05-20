#pragma once // Đảm bảo tệp chỉ được bao gồm một lần trong quá trình biên dịch
#include "GameObject.h" // Kế thừa từ lớp đối tượng chung
#include "Mario.h" // Sử dụng để tương tác với Mario
#include "EdgeSensor.h" // Cảm biến phát hiện cạnh của nền

// Định nghĩa các hằng số liên quan đến trạng thái và thuộc tính của rùa
#define TURTLE_GRAVITY 0.002f // Gia tốc trọng lực tác dụng lên rùa
#define TURTLE_WALKING_SPEED 0.05f // Tốc độ di chuyển khi rùa đi bộ

#define TURTLE_BBOX_WIDTH 16 // Chiều rộng hitbox của rùa
#define TURTLE_BBOX_HEIGHT 14 // Chiều cao hitbox của rùa khi đang đi bộ
#define TURTLE_BBOX_HEIGHT_DIE 7 // Chiều cao hitbox khi rùa ở trạng thái vỏ (shell)

#define ID_ANI_TURTLE_WALKING_RIGHT 5200 // ID animation khi rùa đi sang phải
#define ID_ANI_TURTLE_WALKING_LEFT 5201 // ID animation khi rùa đi sang trái
#define ID_ANI_TURTLE_SHELL 5202 // ID animation khi rùa trong trạng thái vỏ

// Các trạng thái của rùa
#define TURTLE_STATE_WALKING 100 // Trạng thái đi bộ
#define TURTLE_STATE_SHELL 300 // Trạng thái vỏ (đứng yên)
#define TURTLE_STATE_SHELL_MOVING 400 // Trạng thái vỏ đang di chuyển
#define TURTLE_STATE_HELD 500 // Trạng thái rùa bị Mario giữ
#define TURTLE_STATE_REVIVING 600 // Trạng thái rùa hồi sinh
#define TURTLE_STATE_DIE_FALL 700

#define TURTLE_REVIVE_TIMEOUT 5000 // Thời gian hồi sinh sau khi ở trạng thái vỏ
#define TURTLE_SHELL_SLIDE_SPEED 0.15f
class CMario; 
class CTurtle : public CGameObject {
protected:
    float ax, ay; // Gia tốc theo trục x và y
    int walkingDirection; // Hướng di chuyển của rùa
    bool isBeingHeld; // Kiểm tra xem rùa có bị Mario giữ hay không
    ULONGLONG shell_start; // Thời điểm bắt đầu trạng thái vỏ

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom); // Xác định hitbox
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject); // Cập nhật trạng thái mỗi frame
    virtual void Render(); // Vẽ rùa lên màn hình
    virtual int IsCollidable() { return 1; } // Kiểm tra có thể va chạm không
    virtual int IsBlocking() { return 0; } // Kiểm tra có cản đường Mario không
    virtual void OnNoCollision(DWORD dt); // Xử lý khi không có va chạm

    virtual void OnCollisionWith(LPCOLLISIONEVENT e); // Xử lý khi có va chạm
    CEdgeSensor* edgeSensor; // Cảm biến để phát hiện mép
    bool sensorHasFallen; // Kiểm tra cảm biến có bị rơi không
    int pointIdStomp;
    int pointIdKick;
    bool hasSpawnedPointOnStomp = false;
    bool hasSpawnedPointOnKick = false;
    CMario* holder = nullptr;

public:
    CTurtle(float x, float y, int pointIdStomp, int pointIdKick);
    virtual void SetState(int state); // Thiết lập trạng thái
    bool checkMario(); // Kiểm tra tương tác với Mario
    void StartShell(); // Đặt rùa vào trạng thái vỏ
    void KickShell(int dir); // Đá rùa đi theo một hướng
    bool IsShellState(); // Kiểm tra rùa có đang ở trạng thái vỏ không
    bool IsBeingHeld() {return isBeingHeld;}   
    float getvx() { return vx; } // Lấy vận tốc theo trục x
    void SetBeingHeld(bool held) { isBeingHeld = held; }
	float GetX() { return x; } 
	float GetY() { return y; }
    void SetHolder(CMario* mario);
    CMario* GetHolder();
    void SetVx(float vx) { this->vx = vx; }
    void SetVy(float vy) { this->vy = vy; }
	void SetAy(float ay) { this->ay = ay; }
};