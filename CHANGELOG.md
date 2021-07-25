# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [0.2.0](https://github.com/Nick-Fanelli/HarmonyEngine/compare/v0.1.0...v0.2.0) (2021-07-25)


### ⚠ BREAKING CHANGES

* optimize the assets dock to run much faster

### Features

* add ability to change to different transformation modes ([b847ccf](https://github.com/Nick-Fanelli/HarmonyEngine/commit/b847ccfb641c3c2843cfe6141954be0b7795f84c))
* add ability to close all dynamic panels ([e4e7ef8](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e4e7ef849fa97074c826ddc3b0a2b0ccde0b622b))
* add ability to show or hide different editor panels from menu bar ([37ffd3b](https://github.com/Nick-Fanelli/HarmonyEngine/commit/37ffd3b6cfd07161a177a0b4403a2fbb4baf6b90))
* add asset panel to panels menu item ([6c3afe5](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6c3afe59ade817918fdf59310a70d7072d99bb69))
* add cache system for last open project ([3602648](https://github.com/Nick-Fanelli/HarmonyEngine/commit/3602648fa50d71dcbbf5300297ecf05e952ed17b))
* add close buttons on all editor panels ([0adfabe](https://github.com/Nick-Fanelli/HarmonyEngine/commit/0adfabe42ff035ad3bffcb1dfd08a1081ac8bed6))
* add component panel ([f0eb377](https://github.com/Nick-Fanelli/HarmonyEngine/commit/f0eb377033aa8505e63005c3ef4d4ca4c0d1a18e))
* add delete selectable in assets editor panel ([115e566](https://github.com/Nick-Fanelli/HarmonyEngine/commit/115e5667767376af5be2ab3c90940d36852807ea))
* add draw vec2 support to ImGuiLayer ([843bba4](https://github.com/Nick-Fanelli/HarmonyEngine/commit/843bba478f970373222b9923f844478067d39053))
* add event system ([3d3f4df](https://github.com/Nick-Fanelli/HarmonyEngine/commit/3d3f4dff7e5449f236f30a44b6fabc8bbcd8e2d7))
* add gizmo to move object's transform ([75668dd](https://github.com/Nick-Fanelli/HarmonyEngine/commit/75668dd68b1176abd8e49be2757259137256d475))
* add input style to settings ([b500650](https://github.com/Nick-Fanelli/HarmonyEngine/commit/b500650b332e73f2e56d8d51ca66e017e36c38bb))
* add more specs to renderer stats ([028beef](https://github.com/Nick-Fanelli/HarmonyEngine/commit/028beef11eea74a012cf14de7355b0f576ee7016))
* add panel configuration serializationg ([5f6a35e](https://github.com/Nick-Fanelli/HarmonyEngine/commit/5f6a35e32a97d6aedc4bf6981b1603b9f1eb3fd2))
* add project system ([a87eb36](https://github.com/Nick-Fanelli/HarmonyEngine/commit/a87eb3687aae3c6cf02035c6a920fb4dc9fec23f))
* add setting to toggle if current project gets cached ([6d511b8](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6d511b80c73b3bc1271a1b651ae3cad51e12cce5))
* add sprite renderer component ([9af389e](https://github.com/Nick-Fanelli/HarmonyEngine/commit/9af389e7c62dd1f8c62486abe01c8cbf41ab1de6))
* add support for unknown filetype ([4aa8669](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4aa866957d0eb428b2c0d88998daa08da131b660))
* add support to toggle on and off most accessory panels ([68ebd78](https://github.com/Nick-Fanelli/HarmonyEngine/commit/68ebd7870d6341c495d889715c3012b3b9316588))
* add translate, rotate, and scale buttons to change ImGuizmo operation mode ([bea136e](https://github.com/Nick-Fanelli/HarmonyEngine/commit/bea136e60b7f7fe9212e5f926bc0e68ac28e8dea))
* allow for matrix to decompose into position, rotation, and scaleg ([4f6402f](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4f6402fbf7b5e0916cf7a9f08e3d0663aa4a4468))
* assets editor panel ([fd9ebff](https://github.com/Nick-Fanelli/HarmonyEngine/commit/fd9ebff9cd1622ac6ba3287adfe384cf595dfd7e))
* cache current scene in project ([e6cd8b8](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e6cd8b86ed9e29736d7ce904cbc1e01d7d1c11b5))
* display only the shortened asset name instead of the entire absoule asset path ([c1a16a4](https://github.com/Nick-Fanelli/HarmonyEngine/commit/c1a16a45cc7c8cebd5ce3d83e334b2557c6079b6))
* game viewport ([329180d](https://github.com/Nick-Fanelli/HarmonyEngine/commit/329180d1d967c32dd520a9e675f70052b6d09961))
* hide all other panels on tab ([4bbc8f5](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4bbc8f5fd622c74c0a1f6e6bffb0691515f6b834))
* hierarchy panel ([778c21a](https://github.com/Nick-Fanelli/HarmonyEngine/commit/778c21a6cc484c9698417c2d0f8fbc0af43c4adb))
* ImGuizmo controls ([43241c7](https://github.com/Nick-Fanelli/HarmonyEngine/commit/43241c729e3be9cd1d7446b4da4382c62de7ba1b))
* load scene support ([e1d8fde](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e1d8fde2392593a420f84fe9f5ac57a20910e2fc))
* redesign the hiearchy panel to have a search and plus button ([f2de67c](https://github.com/Nick-Fanelli/HarmonyEngine/commit/f2de67cb1008f3fd7d5a4b760af19db7d7509323))
* save as menu button ([ec4f693](https://github.com/Nick-Fanelli/HarmonyEngine/commit/ec4f693128ddc3ddc842e4e3aa35f5b3c528468b))
* save scene on close ([255d19a](https://github.com/Nick-Fanelli/HarmonyEngine/commit/255d19aa1773e644ae3ebdd619926c5d08d2506b))
* scene serialization (load and save scene file into editor ([8a2dee3](https://github.com/Nick-Fanelli/HarmonyEngine/commit/8a2dee3d458a6ac03769172222865a0e451982b8))
* settings system ([30cc0e5](https://github.com/Nick-Fanelli/HarmonyEngine/commit/30cc0e59c638e013a3891ecc276a6c73c7c385c0))
* support more obj formats ([008ba59](https://github.com/Nick-Fanelli/HarmonyEngine/commit/008ba593ad5ffca98996fd78a501157e5555b3fb))


### Bug Fixes

* add shader unbind to the renderer's debug unbind section ([99916f7](https://github.com/Nick-Fanelli/HarmonyEngine/commit/99916f76ae50319054be98945672642cf60a8060))
* fix the event system and make it work ([0d1389f](https://github.com/Nick-Fanelli/HarmonyEngine/commit/0d1389f99a017f4e7b0a7248aba91183dd0f2948))
* force assets to be held in the registry instead of raw assets ([c795f83](https://github.com/Nick-Fanelli/HarmonyEngine/commit/c795f831a83a3d04c14c325e88b60f6b8b4b4d2f))
* load scene name on deserialize ([e9174c5](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e9174c53ad4ca1e254213214362086c12967d5f9))
* override already existant components during scene deserialiation instead of returning ([c037944](https://github.com/Nick-Fanelli/HarmonyEngine/commit/c0379448ec4f6f8be20f4491ecb09f5205638076))
* serialize all entities in the right order ([03256ac](https://github.com/Nick-Fanelli/HarmonyEngine/commit/03256acaa7d0503083dd6d58585a0fefc851ae4f))


* optimize the assets dock to run much faster ([4bd6747](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4bd6747e03a663a87d0628a3e2c4e48c70044ccb))

## 0.1.0 (2021-07-03)


### ⚠ BREAKING CHANGES

* migrated glfw to being a git submodule
* migrate the rest of the code base to headers and cpp files
* migrate dependent namespaces to header and cpp files

### Features

* 3d rendering with lighting ([46c47a2](https://github.com/Nick-Fanelli/HarmonyEngine/commit/46c47a29e627146aa8cf1f07c116300e4814e9ac))
* ability to add new components to entities ([41bc79f](https://github.com/Nick-Fanelli/HarmonyEngine/commit/41bc79fff609db54cef03e24d98dc1b7e59dd208))
* ability to drag and drop textures to mesh renderer and quad renderer components ([696ad6a](https://github.com/Nick-Fanelli/HarmonyEngine/commit/696ad6ae2d0b0048eeb4cdcbbf46851bb9b68945))
* ability to load textures in the asset manager ([a1496fa](https://github.com/Nick-Fanelli/HarmonyEngine/commit/a1496fa588a7c8592cc587941a586b45c1b2179f))
* abstract render stuff away to RenderLayer ([78555f0](https://github.com/Nick-Fanelli/HarmonyEngine/commit/78555f0181ca184eb6516fa437aece65c460e8ae))
* add and delete entities from the editor ([6f20eb0](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6f20eb0be171f191bfe581428225c6c8635aa5e0))
* add color tint to mesh renderer ([c4d69e8](https://github.com/Nick-Fanelli/HarmonyEngine/commit/c4d69e8b6fedfa1de2af0ba641907acf54543035))
* add create new scene button ([398e87a](https://github.com/Nick-Fanelli/HarmonyEngine/commit/398e87a37d55a12cf0b9f87377a566fe421d9dcd))
* add function profiling support ([6081ea1](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6081ea1bd42fb5ccb3ff682e382ae408a88ae5b5))
* add indices to the renderer stats ([e3ecde8](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e3ecde8ac6d36c59ee023e7ca3ac9e00b7c04866))
* add lighting and texturing to work at the same time ([b1f8afd](https://github.com/Nick-Fanelli/HarmonyEngine/commit/b1f8afd932bb8e1ee6e1108c04c6c0eb735612c1))
* add mesh struct ([8728c1f](https://github.com/Nick-Fanelli/HarmonyEngine/commit/8728c1f62982bcb45c3e1c349911fddede30c747))
* add new project popup and dialog ([0bc7ef0](https://github.com/Nick-Fanelli/HarmonyEngine/commit/0bc7ef085a39fe7d1c7bc27af282bcdd166be66a))
* add package.json npm for conventional commits ([3def284](https://github.com/Nick-Fanelli/HarmonyEngine/commit/3def2847d1bc6a3619d1358ef8351d09c688b2b5))
* add quad renderer component to drawable components ([09a1727](https://github.com/Nick-Fanelli/HarmonyEngine/commit/09a1727ef62782fe18f477817dc63d1c2b369fa0))
* add seperation of projects ([e19673b](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e19673b16ad0a28cc960696f12540b76f73bfcc7))
* add texture class ([bc66b69](https://github.com/Nick-Fanelli/HarmonyEngine/commit/bc66b69269e17ea752c6d95fac0ad371e4eec641))
* all assets from asset dock support ([9389374](https://github.com/Nick-Fanelli/HarmonyEngine/commit/9389374f536c2a051fe3df5e851fefcb8c74b691))
* assertions ([2648ba8](https://github.com/Nick-Fanelli/HarmonyEngine/commit/2648ba8ddbae8d8d0e0c7a62b6a6597861299af4))
* baisc 2D rendering ([f5351b3](https://github.com/Nick-Fanelli/HarmonyEngine/commit/f5351b3c5e4605cd13f85e1cb2b7b8a0019c1d6c))
* basic 3d renderer ([e6df0d7](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e6df0d730fa94cc708f9d8f44de5324deb9cfc34))
* basic 3d renderer ([a2a1174](https://github.com/Nick-Fanelli/HarmonyEngine/commit/a2a11740a8d1d47b7c8cf8ff6e89dd41da1de497))
* basic implementation of ImGUILayer ([2800558](https://github.com/Nick-Fanelli/HarmonyEngine/commit/28005589d342291902435ddf2cb758c991508152))
* basic scene manager and scene ([ab3bc5e](https://github.com/Nick-Fanelli/HarmonyEngine/commit/ab3bc5ed3ea6aa800b21aafa663e3ca425719c92))
* basic scene serialization ([10514bf](https://github.com/Nick-Fanelli/HarmonyEngine/commit/10514bf275dab77166f9fb14de487712c19756e4))
* basic terrain generation ([9902fcf](https://github.com/Nick-Fanelli/HarmonyEngine/commit/9902fcf6995da221c65fdc8ecc7bcbbe39949157))
* better imgui ini loading to custom file ([d9e55d2](https://github.com/Nick-Fanelli/HarmonyEngine/commit/d9e55d2f411365602f183b8b5e61e7f121c06a44))
* better imgui ini loading to custom file ([3bcf6d8](https://github.com/Nick-Fanelli/HarmonyEngine/commit/3bcf6d8cb4993d3edce373cb053b454075e4a503))
* change the movement controller to work similar to minecraft ([6efc7b2](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6efc7b2d1adba2d4722f6ede3bf2029135b60bf8))
* complete working model and texture loading system ([2d41bef](https://github.com/Nick-Fanelli/HarmonyEngine/commit/2d41bef8d470bc31994d399e60a512afa662eb66))
* custom font ([75b9e67](https://github.com/Nick-Fanelli/HarmonyEngine/commit/75b9e67efbbb9e5152fec6e8a6e01bad9768824c))
* default shaders ([bbc6bab](https://github.com/Nick-Fanelli/HarmonyEngine/commit/bbc6bab450c6008b0af2f11cdc0306c51bc845d3))
* delete file from assets manager ([b0e9826](https://github.com/Nick-Fanelli/HarmonyEngine/commit/b0e9826becbe92209132f15fa3b23b5aa242ad58))
* dockspace ([2e06772](https://github.com/Nick-Fanelli/HarmonyEngine/commit/2e06772d3cef96e54587384b31c8e8cf0fc36f60))
* dynamically load the assets file of the current project ([d7d8999](https://github.com/Nick-Fanelli/HarmonyEngine/commit/d7d89997f9d3dd91a90805a1af6a0dea18a296a8))
* dynamically load varibales into shader on Shader::Create() ([f360499](https://github.com/Nick-Fanelli/HarmonyEngine/commit/f360499f1f426f3761ef36f8a3d63187e80fcc2d))
* editor camera ([eff8d32](https://github.com/Nick-Fanelli/HarmonyEngine/commit/eff8d322f00d60c616a3b3b0b2405bc33c45dc92))
* entity component system ([5c77c48](https://github.com/Nick-Fanelli/HarmonyEngine/commit/5c77c4862e125f12a8e4e2f4bed559c4de3301d8))
* environmental lighting controls ([4c197e4](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4c197e417fc2f33d93acae3238b5e47acdf1724a))
* first person camera ([819ab4c](https://github.com/Nick-Fanelli/HarmonyEngine/commit/819ab4c9b2b0e0571c311f204e2fdffb36c84ed2))
* font-awesome support ([b740adb](https://github.com/Nick-Fanelli/HarmonyEngine/commit/b740adbd00600965321fb651b907d360ca22ea3f))
* format logging ([3fc7ed4](https://github.com/Nick-Fanelli/HarmonyEngine/commit/3fc7ed4e4a5e7218b73940cbd4acfaedca32ba16))
* framebuffer class support ([0bbd426](https://github.com/Nick-Fanelli/HarmonyEngine/commit/0bbd426fbcc8a63eb41b0d56c99d5b5a10e09b5d))
* framebuffer viewport for rendering scene ([6f0f5d5](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6f0f5d5375545dc74ed00f5ba59d37b31117c0f3))
* game scene ([98d3ddf](https://github.com/Nick-Fanelli/HarmonyEngine/commit/98d3ddf154e5b75b10ce3aba713bd3ce9dc399e6))
* glfw display ([3bbb799](https://github.com/Nick-Fanelli/HarmonyEngine/commit/3bbb799e7d0946eede0b8f8859b0d554baafd276))
* hierarchy editor panel ([0364805](https://github.com/Nick-Fanelli/HarmonyEngine/commit/03648054f4505b731752139e4197726832ffee48))
* implement auto mesh renderer entity rendering ([fb3aa3a](https://github.com/Nick-Fanelli/HarmonyEngine/commit/fb3aa3a6248989862c6df9a1199ca12770b242d4))
* implement master renderer to control the viewport ([99da2d7](https://github.com/Nick-Fanelli/HarmonyEngine/commit/99da2d7136b64313ebe7d688d082c42ae8544f75))
* input system ([a8af6de](https://github.com/Nick-Fanelli/HarmonyEngine/commit/a8af6de448ad9702f26d94bd9c40aa2c937b0afa))
* inspector panel ([c3c0235](https://github.com/Nick-Fanelli/HarmonyEngine/commit/c3c023521e4ebd342d65e489aa3b56d836a34c49))
* instanced offsets for objects in Renderer3D ([6ce837f](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6ce837f76c9bce2a0d8ccc63e4118ca387c54f43))
* last active project cache ([d40a2f3](https://github.com/Nick-Fanelli/HarmonyEngine/commit/d40a2f35dc841d610514d91105a43b89f36363b1))
* lighting dynamically reacts to the objects transformations ([a6c03c4](https://github.com/Nick-Fanelli/HarmonyEngine/commit/a6c03c4256b9576269f1e882ba885a60f7d1a63b))
* log namespace ([4534a98](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4534a989d00515b18c265c0101149dadeefbdf42))
* menu bar ([e446a01](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e446a01614b038a79a9661c762d20ce71b7cc0f5))
* mesh drag and drop control ([9e34f33](https://github.com/Nick-Fanelli/HarmonyEngine/commit/9e34f33793d9316fa2f8f1edfd9f6a8d432b5175))
* mesh renderers can now take in textures ([cf930b3](https://github.com/Nick-Fanelli/HarmonyEngine/commit/cf930b316f5e21cdd5718158a3ed282a25f4c860))
* open and load project data ([8f7f0e6](https://github.com/Nick-Fanelli/HarmonyEngine/commit/8f7f0e626e4523031437112dfb86c7b0b7081caa))
* orthographic camera ([2748e4f](https://github.com/Nick-Fanelli/HarmonyEngine/commit/2748e4f29c7440ff05da2f5a1fd0be0b9f22065b))
* perlin noise generated height map for terrain ([d1837e5](https://github.com/Nick-Fanelli/HarmonyEngine/commit/d1837e5fff954f32854e67fcb07bf6558e045e8c))
* renderer stats ([564d9b1](https://github.com/Nick-Fanelli/HarmonyEngine/commit/564d9b1d82c23258ff754aa7c73f33109b5b006b))
* right click delete button on hiearhcy item ([fa952e2](https://github.com/Nick-Fanelli/HarmonyEngine/commit/fa952e2a00f91f74c84056bf2eef79f552158030))
* rotational quad rendering support ([62fb189](https://github.com/Nick-Fanelli/HarmonyEngine/commit/62fb189d90822b1ef8cae76523252478472c1365))
* save and load settings file from application support ([7acd4c9](https://github.com/Nick-Fanelli/HarmonyEngine/commit/7acd4c9e19f9d9f54a671f0234e2c9ca8644345b))
* save editor layout ini file to the application support directory ([6fe5b9e](https://github.com/Nick-Fanelli/HarmonyEngine/commit/6fe5b9ecc86c06edfea79c229012c5b542f368dd))
* settings window ([a6da1b1](https://github.com/Nick-Fanelli/HarmonyEngine/commit/a6da1b129bb9372b4274c3919df14e1521a5508e))
* shader support ([fd3b5dd](https://github.com/Nick-Fanelli/HarmonyEngine/commit/fd3b5ddcf5fd9acf24b644eab5103a319b2efdbe))
* support for profiling major functions ([030762b](https://github.com/Nick-Fanelli/HarmonyEngine/commit/030762bf580095604be0d03b456eca0624673853))
* support more than GL_MAX_TEXTURES in the Renderer class (not tested) ([2dd07cd](https://github.com/Nick-Fanelli/HarmonyEngine/commit/2dd07cd79c246414821185f5607b09c13e00f4da))
* texture and mesh serialization ([d8f2c2a](https://github.com/Nick-Fanelli/HarmonyEngine/commit/d8f2c2a2188a49590cc20be1fa73b308203174fb))
* texture loading via the asset manager ([2da7797](https://github.com/Nick-Fanelli/HarmonyEngine/commit/2da7797ba3d5c13d2c4fed7c5cdc1d5a9c184c4d))
* texture support for 3D models ([38a5c68](https://github.com/Nick-Fanelli/HarmonyEngine/commit/38a5c6831894831e45a554d1d38aaeb92d2981aa))
* theme data system ([4846a25](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4846a258244605660909a16706d7855e799a4036))
* tranform object ([397b917](https://github.com/Nick-Fanelli/HarmonyEngine/commit/397b917f41ee98261f7b03d013c631ca700ffbfe))
* update the color picker to be prettier ([7efe5a6](https://github.com/Nick-Fanelli/HarmonyEngine/commit/7efe5a640bcecb75828fb5d91dd05243e08aa879))


### Bug Fixes

* ability to render more than the maximum amount of OpenGL texture units ([f4e8b90](https://github.com/Nick-Fanelli/HarmonyEngine/commit/f4e8b908be42e3a8d68ef9f1bf9877491c2848b8))
* allow multiple objects to move independently ([dd06387](https://github.com/Nick-Fanelli/HarmonyEngine/commit/dd06387dc33ad317a77054d8770ddbe3a9fca7e9))
* allow the Renderer2D to take its own shaders ([d57ad34](https://github.com/Nick-Fanelli/HarmonyEngine/commit/d57ad34494487626846cc6746036a26c373fd6d3))
* allow the transform component to be removed and added ([c58d169](https://github.com/Nick-Fanelli/HarmonyEngine/commit/c58d1696b0907b434a2f4f5fc633091e74533ba5))
* asset dock ([c220104](https://github.com/Nick-Fanelli/HarmonyEngine/commit/c22010466e69502a0957f07cb75ab240c244a612))
* change the system to use asset handle instead of asset ([1a09606](https://github.com/Nick-Fanelli/HarmonyEngine/commit/1a09606c9d3a60fa2a9b1604b2c314f6a46f4317))
* force creation of entity through scene ([0ebc75e](https://github.com/Nick-Fanelli/HarmonyEngine/commit/0ebc75ee70360c26abe8163b3c1ae54f865f7891))
* imgui id conflict ([8472761](https://github.com/Nick-Fanelli/HarmonyEngine/commit/8472761e49de479895e11499c5ce8cfdb0aa353a))
* only allow editor scene camera movement when the scene editor is selected ([4723f00](https://github.com/Nick-Fanelli/HarmonyEngine/commit/4723f007063695e72d9addc5362d845c5aee40e1))
* scene clipping and viewport resizing ([cb65a27](https://github.com/Nick-Fanelli/HarmonyEngine/commit/cb65a27c9a8fdb000ff7845c44dfb5982f1ab5ee))
* set and load active scene after opengl and window creation/setup ([ed2eab6](https://github.com/Nick-Fanelli/HarmonyEngine/commit/ed2eab63d81c1b47af46a9d656516667cae0c6a7))
* setting system ([080454a](https://github.com/Nick-Fanelli/HarmonyEngine/commit/080454a82713c0509f11f78992e5e4df31d1feeb))
* typo ([e8bbc5e](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e8bbc5e03682a47cb4d7322e9621bdda38e89338))
* update the .gitignore to ignore the .DS_Store ([e5c1b61](https://github.com/Nick-Fanelli/HarmonyEngine/commit/e5c1b61d87e3b7ccb5b833fcba84efe4cb280664))
* update the editor camera on object creation ([81bb682](https://github.com/Nick-Fanelli/HarmonyEngine/commit/81bb682a7f53920790ccd6f65c5161098feb2236))
* update the mesh input and texture input to use the same theme ([1c05163](https://github.com/Nick-Fanelli/HarmonyEngine/commit/1c0516378009d724043f4c3f79334a049e961858))


* migrate dependent namespaces to header and cpp files ([2a4ef7d](https://github.com/Nick-Fanelli/HarmonyEngine/commit/2a4ef7dcd5fcece000095de0d2c880f2ff8c6f20))
* migrate the rest of the code base to headers and cpp files ([19eecef](https://github.com/Nick-Fanelli/HarmonyEngine/commit/19eecefbfdfed3ca5e05e157e8e77936fd8a9056))
* migrated glfw to being a git submodule ([67414b0](https://github.com/Nick-Fanelli/HarmonyEngine/commit/67414b003d6262fda5de45320d850a628d4c1942))
