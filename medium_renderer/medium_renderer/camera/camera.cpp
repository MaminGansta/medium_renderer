
#include "camera.h"


namespace renderer
{

    gm::mat4 lookAt(const gm::vec3& from, const gm::vec3& to, const gm::vec3& tmp = gm::vec3(0, 1, 0))
    {
        gm::vec3 forward = (from - to).get_normalized();
        gm::vec3 right = tmp.get_normalized() ^ forward;
        gm::vec3 up = forward ^ right;

        gm::mat4 camToWorld;

        camToWorld[0][0] = right.x;
        camToWorld[0][1] = right.y;
        camToWorld[0][2] = right.z;
        camToWorld[1][0] = up.x;
        camToWorld[1][1] = up.y;
        camToWorld[1][2] = up.z;
        camToWorld[2][0] = forward.x;
        camToWorld[2][1] = forward.y;
        camToWorld[2][2] = forward.z;

        camToWorld[0][3] = from.x;
        camToWorld[1][3] = from.y;
        camToWorld[2][3] = from.z;

        return camToWorld;
    }

        // Constructor with vectors
        Camera::Camera(const gm::vec3& position, const gm::vec3& up, float yaw, float pitch)
            :
            Front(gm::vec3(1.0f, 0.0f, 0.0f)),
            MovementSpeed(SPEED),
            MouseSensitivity(SENSITIVTY),
            Zoom(ZOOM)
        {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }


        // Constructor with scalar values
        Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
            :
            Front(gm::vec3(0.0f, 0.0f, -1.0f)),
            MovementSpeed(SPEED),
            MouseSensitivity(SENSITIVTY),
            Zoom(ZOOM)
        {
            Position = gm::vec3(posX, posY, posZ);
            WorldUp = gm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }


        // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
        gm::mat4 Camera::get_lookat()
        {
            return gm::lookat(Position, Position + Front);
            //return gm::lookat(Position, Position + Front);
            //return lookAt(Position, Position + Front, Up);
        }


        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
        {
            float velocity = MovementSpeed * deltaTime;
            if (direction == FORWARD)
                Position -= Front * velocity;
            if (direction == BACKWARD)
                Position += Front * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;
        }


        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw -= xoffset;
            Pitch -= yoffset;

            // Make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            // Update Front, Right and Up Vectors using the updated Eular angles
            updateCameraVectors();
        }


        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void Camera::ProcessMouseScroll(float yoffset)
        {
            if (Zoom >= 1.0f && Zoom <= 45.0f)
                Zoom -= yoffset;
            if (Zoom <= 1.0f)
                Zoom = 1.0f;
            if (Zoom >= 45.0f)
                Zoom = 45.0f;
        }


        // Calculates the front vector from the Camera's (updated) Eular Angles
        void Camera::updateCameraVectors()
        {
            // Calculate the new Front vector
            gm::vec3 front;
            front.x = cosf(gm::radians(Yaw)) * cosf(gm::radians(Pitch));
            front.y = sinf(gm::radians(Pitch));
            front.z = sinf(gm::radians(Yaw)) * cosf(gm::radians(Pitch));
            Front = front.get_normalized();
            // Also re-calculate the Right and Up vector
            Right = (Front ^ WorldUp).get_normalized();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            Up = (Right ^ Front).get_normalized();
        }

}