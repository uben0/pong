#ifndef KTimer_HEADER_DEFINED
#define KTimer_HEADER_DEFINED

/// \brief A class the manage time in engine loop.
///
/// Make sure a loop last the specified time to ensure constant FPS.
class KTimer {
	unsigned int mMsPerLoop;
	unsigned long long mCount;
	unsigned long long mStart;
public:
	/// \brief Predefined amount of millisecond to match 60 FPS.
	///
	static constexpr unsigned int Fps60 = 1000/60;

	/// \brief Predefined amount of millisecond to match 30 FPS.
	///
	static constexpr unsigned int Fps30 = 1000/30;

	/// \brief Read only amount of millisecond per loop.
	///
	const unsigned int& ms = mMsPerLoop;

	/// \brief Read only amount of performed loops.
	///
	const unsigned long long& count = mCount;

	/// \brief Initialize with the amount of millisecond per loop.
	///
	KTimer(unsigned int msPerLoop = Fps60);

	/// \brief Reset the class.
	///
	/// \param msPerLoop the amount of millisecond per loop.
	void start(unsigned int msPerLoop);

	/// \brief Reset the class.
	///
	void start();

	/// \brief Wait until the loop complete its time.
	///
	void wait();

	/// \brief Get the amount of millisecond passed.
	///
	unsigned long long get_time() const;

	/// \brief Wait for a lapse of time.
	///
	/// \param msToWait the amount of millisecond to wait.
	static void wait_for(unsigned int msToWait);

	/// \brief Print time as YYYY-MM-DD hh:mm:ss
	///
	static std::string date();
};

#endif
