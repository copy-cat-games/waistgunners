const Electron = require("electron");

var window;

Electron.Menu.setApplicationMenu(null);

function create_window() {
    window = new Electron.BrowserWindow({
        width: 400, height: 600,
        webPreferences: {
            nodeIntegration: false,
        },
        show: false,
    });

    window.loadFile("index.html");
    window.on("ready-to-show", window.show);
    window.on("closed", () => {
        window = null;
    });
}

Electron.app.whenReady().then(create_window);

Electron.app.on("window-all-closed", () => {
    Electron.app.quit();
});

function open_file(file) {
    // maybe for highscores?
    console.log("opening file: " + file);
    var contents = fs.readFileSync(file).toString();
    window.webContents.send("file opened", file, contents);
}