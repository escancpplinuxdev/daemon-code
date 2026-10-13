
#include <iostream>
#include <vector>
#include <memory>
#include <string>


class AudioSource
{
	public:
    // Virtual destructor is crucial to prevent memory leaks in derived classes
	virtual ~AudioSource() = default;

    // Pure virtual function making this a dynamic interface
	virtual void play() const = 0;
	
	
};

// Derived Class 1
class MP3Audio : public AudioSource
{
	public:
	void play() const override 
	{
        	std::cout << "[Playing] Streaming compressed MP3 audio track...\n";
	}
};

// Derived Class 2
class WAVAudio : public AudioSource
{
	public:
	void play() const override
	{
	        std::cout << "[Playing] Playing raw, uncompressed WAV audio track...\n";
	}
};

// 2. COMPILE-TIME POLYMORPHISM (Static Binding)

class AudioEngine
{
	public:
	// A. FUNCTION OVERLOADING (Static Polymorphism)
    // The compiler resolves which function to call based on the parameter type
  
	void adjustVolume(int percentage)
{
	        std::cout << "[Volume Control] Setting system volume to: " << percentage << "%\n";
}

	void adjustVolume(const std::string& profile)
{
	        std::cout << "[Volume Control] Applying equalizer profile presets: " << profile << "\n";
}

// B. TEMPLATES (Parametric Static Polymorphism)
// Allows compile-time code generation for processing any valid audio stream

template <typename T>
void processEffects(const T& audioEffect)
{
	        std::cout << "[DSP Engine] Applying effect processing sequence...\n";
	audioEffect.apply();// Resolved statically at compile time
  
}
};   

// Custom Effect structural type for the template
struct EchoEffect
{
	void apply() const { std::cout << " -> Loop-back delay echo applied successfully.\n"; }
};




int main()
{
	AudioEngine engine;

	std::cout<<" -- Testing Complie-time Polymorphism --\n";

	engine.adjustVolume(85);
	engine.adjustVolume("Bass Bost");
	
	//Template parameter resolution

	EchoEffect echo;
	engine.processEffects(echo);

	std::cout<<" ---Testing Runtime Polymorphism --\n";
    // Using modern std::unique_ptr for safe dynamic memory allocation	
	std::vector<std::unique_ptr<AudioSource>> playlist;
	playlist.push_back(std::make_unique<MP3Audio>());
	playlist.push_back(std::make_unique<WAVAudio>());

	// The compiler doesn't know what type 'track' is until execution time.
    // It dynamically looks up the target address inside the virtual table (VTABLE).
    	
	for (const auto& track: playlist)
	{
		track->play();
	}
	return 0;
}
