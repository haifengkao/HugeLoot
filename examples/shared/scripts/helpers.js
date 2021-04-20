// Helper functions shared across scripts.

const childProcess = require('child_process');
const path = require('path');
const fs = require('fs');
const os = require('os');

function fileExists(filePath) {
    try {
        // Query the entry
        const stats = fs.lstatSync(filePath);

        if (os.platform() === 'darwin') {
            // xxx.app is a folder in osx
            if (stats.isDirectory()) {
                return true
            }

        } else if (stats.isFile()) {
            // Is it a directory?
            return true;
        }
    } catch (e) {
        /* Don't do anything, it's not an error. */
    }

    return false;
}

function getBinaryParentPaths(rootPath) {
    const paths = [
        {
            path: path.join(rootPath, 'release'),
            label: null
        },
        {
            path: path.join(rootPath, 'debug'),
            label: null
        },

        // windows location
        {
            path: path.join(rootPath, '32', 'release'),
            label: '32-bit'
        },
        {
            path: path.join(rootPath, '64', 'release'),
            label: '64-bit'
        },
        {
            path: path.join(rootPath, '32', 'debug'),
            label: '32-bit'
        },
        {
            path: path.join(rootPath, '64', 'debug'),
            label: '64-bit'
        }
    ];

    return paths;
}

function getAppReleasePaths(rootPath) {
    let file = 'HugeLoot';
    if (os.platform() === 'win32') {
        file += '.exe';
    }
    if (os.platform() === 'darwin') {
        file += '.app';
    }

    return getBinaryParentPaths(rootPath).filter(parentPath =>
        fileExists(path.join(parentPath.path, file))
    );
}

function safeExecFileSync(file, args, options) {
    try {
        return childProcess.execFileSync(file, args, options);
    } catch (error) {
        throw new Error(error.message);
    }
}

module.exports.fileExists = fileExists;
module.exports.getAppReleasePaths = getAppReleasePaths;
module.exports.safeExecFileSync = safeExecFileSync;
