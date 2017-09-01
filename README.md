# MayaToGameplay3D
Real-time viewer for Maya

This is a project developed by two students (https://github.com/Fanny94, https://github.com/Raxo94). 

The project is a real-time communication mechanism between Maya and a rendering engine using message passing through
shared memory. Conceptually, changes of a 3D model in Maya will be seen and updated almost immediately in the renderer in real-time.

the project is divided in two executable programs; ImportNodesFromMaya and MayaImporter.

   -ImportNodesFromMaya: Listens to callback from Maya (using Maya CPP API) and sends that information to the other program (renderer) in real-time. The information is sent by the circular buffer, constantly pushing messages containing the values from the 3D model created in Maya.

   -MayaImporter: Recieves the information in runtime by the messages from the circluar buffer. The program includes a renderer (using Gameplay 3D) that renders the information by creating a scnene and storing the information in a list of objects to be rendered. 

Each message contains a header that contains different information depending on which type of object that is suppose to be rendered. This header has to look the same in both of the programs for the MayaImporter to be able to recieve the correct size of information.

The system supports the following in real-time: 
- Creating/adding 3D models 
- Transforming (scaling, translation, rotation) a 3D model
- Transformation of vertices, faces and edges
- Moving the camera viewer
- Swifting different camera views 

For more information about the concept of a circular buffer and the maya plugin look at my other github projects: https://github.com/Fanny94/Pipeline_tool-Sharing_memory, https://github.com/Fanny94/Maya_Plugin 
