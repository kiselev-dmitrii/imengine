#include "StreamUtils.h"

using std::endl;

namespace imCore {

std::ostream &StreamUtils::operator<<(std::ostream &stream, const IVec2 &vec) {
        stream << "(" << vec.x << ", " << vec.y << ")";
        return stream;
}

std::ostream &StreamUtils::operator<<(std::ostream &stream, const Vec2 &vec) {
        stream << "(" << vec.x << ", " << vec.y << ")";
        return stream;
}

std::ostream &StreamUtils::operator<<(std::ostream &stream, const Vec3 &vec) {
        stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return stream;
}

std::ostream &StreamUtils::operator<<(std::ostream &stream, const Vec4 &vec) {
        stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return stream;
}

std::ostream &StreamUtils::operator<<(std::ostream &stream, const Mat3 &mat) {
        stream << endl;
        stream << "|" << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << "|" << endl;
        stream << "|" << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << "|" << endl;
        stream << "|" << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << "|" << endl;
        return stream;
}

std::ostream &StreamUtils::operator<<(std::ostream &stream, const Mat4 &mat) {
        stream << endl;
        stream << "|" << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << "|" << endl;
        stream << "|" << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << "|" << endl;
        stream << "|" << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << "|" << endl;
        stream << "|" << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << "|" << endl;
        return stream;
}

std::ostream &StreamUtils::operator<<(std::ostream &stream, const Quat &q) {
        stream << "(" << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3] << ")";
        return stream;
}

} //namespace imCore
