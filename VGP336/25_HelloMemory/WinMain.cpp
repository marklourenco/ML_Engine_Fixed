#include <Inc/ML_Engine.h>

using namespace ML_Engine;
using namespace ML_Engine::Core;

static int sUniqueIdStudent = 0;
static int sUniqueIdTeacher = 0;
static int sUniqueIdStaff = 0;
class Student
{
public:
		Student()
		{
			mId = ++sUniqueIdStudent;
			mMark = 60 + (rand() % 41);
			mName = "NoName_" + std::to_string(mId);
		}
		Student(const std::string& name)
			: mName(name)
		{
			mId = ++sUniqueIdStudent;
			mMark = 60 + (rand() % 41);
		}
		Student(const std::string& name, int mark)
			: mName(name), mMark(mark)
		{
			mId = ++sUniqueIdStudent;
		}
		void Log()
		{
			LOG("Name: %s, Student ID: %d, Mark: %d", mName.c_str(), mId, mMark);
		}
	private:
		int mId = 0;
		int mMark = 0;
		std::string mName;
};

class Teacher
{
public:
	Teacher()
	{
		mId = ++sUniqueIdTeacher;
		mPrefix = rand() % 2 == 0 ? "Mr." : "Ms.";
		mName = "NoName_" + std::to_string(mId);
	}
	void Log()
	{
		LOG("Name: %s %s, Teacher ID: %d", mPrefix.c_str(), mName.c_str(), mId);
	}
private:
	int mId = 0;
	std::string mName;
	std::string mPrefix;
};

class Staff
{
public:
	Staff()
	{
		mId = ++sUniqueIdStaff;
		mPosition = rand() % 2 == 0 ? "Manager" : "Assistant";
		mName = "NoName_" + std::to_string(mId);
	}
	void Log()
	{
		LOG("Name: %s, Staff ID: %d, Position: %s", mName.c_str(), mId, mPosition.c_str());
	}
private:
	int mId = 0;
	std::string mName;
	std::string mPosition;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);
	TypedAllocator teacherPool = TypedAllocator<Teacher>("TeacherPool", 100);
	TypedAllocator staffPool = TypedAllocator<Staff>("StaffPool", 100);

	std::vector<Student*> students;
	std::vector<Teacher*> teachers;
	std::vector<Staff*> staff;

	// populate pools
	for (uint32_t i = 0; i < 70; ++i)
	{
		std::string newStudentName = "NewStudent" + std::to_string(i);
		Student* newStudent = studentPool.New(newStudentName);
		newStudent->Log();
		students.push_back(newStudent);
	}
	for (uint32_t i = 0; i < 70; ++i)
	{
		Teacher* newTeacher = teacherPool.New();
		newTeacher->Log();
		teachers.push_back(newTeacher);
	}
	for (uint32_t i = 0; i < 70; ++i)
	{
		Staff* newStaff = staffPool.New();
		newStaff->Log();
		staff.push_back(newStaff);
	}

	// delete 40 of each
	for (uint32_t i = 0; i < 40; ++i)
	{
		Student* student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}
	for (uint32_t i = 0; i < 40; ++i)
	{
		Teacher* teacher = teachers.back();
		teacherPool.Delete(teacher);
		teachers.pop_back();
	}
	for (uint32_t i = 0; i < 40; ++i)
	{
		Staff* staffMember = staff.back();
		staffPool.Delete(staffMember);
		staff.pop_back();
	}

	// add new 70 of each
	for (uint32_t i = 0; i < 70; ++i)
	{
		std::string newStudentName = "NewStudent" + std::to_string(i);
		Student* newStudent = studentPool.New(newStudentName);
		newStudent->Log();
		students.push_back(newStudent);
	}

	for (uint32_t i = 0; i < 70; ++i)
	{
		Teacher* newTeacher = teacherPool.New();
		newTeacher->Log();
		teachers.push_back(newTeacher);
	}

	for (uint32_t i = 0; i < 70; ++i)
	{
		Staff* newStaff = staffPool.New();
		newStaff->Log();
		staff.push_back(newStaff);
	}

	// cleanup
	for (Student* student : students)
	{
		student->Log();
		studentPool.Delete(student);
	}
	students.clear();

	for (Teacher* teacher : teachers)
	{
		teacher->Log();
		teacherPool.Delete(teacher);
	}
	teachers.clear();

	for (Staff* staffMember : staff)
	{
		staffMember->Log();
		staffPool.Delete(staffMember);
	}

	return 0;
}