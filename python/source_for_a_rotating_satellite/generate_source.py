import sys

def main():
    """Main function called when executing the script."""
    if len(sys.argv) > 1:
        name = sys.argv[1]
    else:
        name = "ChatGPT"
    
    print("Hello from main!")
    result = helper_function(name)
    print(f"Result from helper_function: {result}")

def helper_function(name):
    """A helper function used inside main."""
    return f"Hello, {name}!"

if __name__ == "__main__":
    main()
