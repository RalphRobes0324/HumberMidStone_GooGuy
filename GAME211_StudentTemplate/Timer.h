#ifndef TIMER_H
#define TIMER_H


	class Timer {
	private:
		unsigned int prevTicks;
		unsigned int currTicks;
	public:
		Timer();
		~Timer();

		void Start();
		void UpdateFrameTicks();
		float GetDeltaTime() const;
		unsigned int GetSleepTime(const unsigned int fps) const;
	};


#endif
