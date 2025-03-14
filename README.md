*****System Requirements*****
Minimum:
    OS: Windows 10 64-bit or newer
    Processor: Six-core Intel or AMD, 2.5 GHz or faster
    Memory: 32 GB RAM
    Graphics: RTX 2060 Super or better
    Storage: 150 GB available space
Recomended:
    OS: Windows 11 64-bit
    Processor: 24 Core 32 Thread Intel I9 13900k @ 5.8GHz
    Memory: 32 GB RAM DDR5 5600
    Graphics: RTX 4080 16GB
    Storage: 2TB NVMe SSD

*****Required Software*****
In order to work on Wild Omission you will need the following software:

GitHub desktop: https://desktop.github.com/download/

Visual Studio 2022 Community: https://visualstudio.microsoft.com/vs/
      Note: Make sure to select "Game Development with C++" in the installer, and on the side also make sure "Unreal Engine Installer" is selected, as it provides additional tools that are useful for working with unreal engine.

Epic Games Launcher: https://store.epicgames.com/en-US/download
      Open the Epic Games Launcher, and go to the Unreal Engine tab. Click on the plus button to add an Unreal Engine installation.
      By default it will select the newest version of the engine. However for our cases Wild Omission uses Unreal Engine 5.3.2, so we will be using this.
      To change version, click on the version number to open a drop down menu, where you can select 5.3.2, and click install. This will take 20 minutes to an hour depending on your internet.

*****Cloning the repo*****
To download the repository. Open GitHub Desktop. Once in GitHub Desktop, click on File from the context menu at the top, and click "Clone Repository...".
Click the "URL" tab and type "TelephoneStudios/WildOmission" into the url text box. Use the local path to choose the folder you want the repository to be put in (Note that the process of cloning will create a Wild Omission
folder, so avoid naming the folder you're putting it in "Wild Omission" to avoid confusion.). Then click the Clone Button.
You should see a loading bar once it starts cloning, it may take a few minutes.

*****Generating Visual Studio Project Files*****
Once the repository has been successfully cloned, navigate to the folder you cloned the repo to.
Right click on the "WildOmission.uproject" file and select "Show More Options" (Windows 11 only), and then "Generate Visual Studio Project Files"

*****Compiling the code*****
Once it's done, you should have a new file called "WildOmission.sln" double click to open it in Visual Studio.
Once open Click the green arrow to build, not the box that says "Local Windows Debugger" as we aren't debugging. This will start the build process. It may take anywhere from 5 minutes to 2 hours depending on your machine.
Once it's done building it will automatically open the Unreal Editior.
Congratulations that's it. Edit
You've build your very own copy of Wild Omission on you own computer. You can now click the green play button at the top to start playing.

*****Packaging*****
To turn the project files into a complete executable, click on the "Platforms" button at the top. Then select "Windows" and then "Package Project". It will then ask you where you want to package the project, and build it.
Once it's done you should have a folder containing all the necessary binaries. You can run WildOmission.exe to play it.
