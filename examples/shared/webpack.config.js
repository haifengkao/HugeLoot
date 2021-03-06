const path = require('path');
const os = require('os');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const { getAppReleasePaths } = require('./scripts/helpers');

const hugeLootBuildPath = path.join(__dirname, '..', '..', 'build');
var releasePath = getAppReleasePaths(hugeLootBuildPath)[0].path;

if (os.platform() === 'darwin') {
    releasePath = path.resolve(releasePath, 'HugeLoot.app', 'Contents', 'MacOS');
    console.log('release:', releasePath);
}

// Constant with our paths
const paths = {
    RESOURCES: path.resolve(__dirname, 'resources', 'ui'),
    DIST: path.resolve(releasePath, 'resources', 'ui'),
    SRC: path.resolve(__dirname, 'src', 'gui', 'html')
};

// Webpack configuration
module.exports = {
    mode: 'production',
    context: path.join(__dirname),
    entry: path.join(paths.SRC, 'js', 'app.ts'),
    output: {
        path: paths.DIST,
        filename: 'app.bundle.js',
        publicPath: ''
    },
    devtool: 'source-map',
    plugins: [
        new HtmlWebpackPlugin({
            template: path.join(paths.SRC, 'index.html')
        }),
        new CopyWebpackPlugin({
            patterns: [
                {
                    from: path.join(paths.SRC, 'css', 'style.css'),
                    to: path.join(paths.DIST, 'css')
                },
                {
                    from: path.join(paths.SRC, 'css', 'typography.css'),
                    to: path.join(paths.DIST, 'css')
                },
                {
                    from: path.join(paths.RESOURCES, 'css', 'dark.theme.css'),
                    to: path.join(paths.DIST, 'css')
                },
                {
                    from: path.join('build', 'fonts'),
                    to: path.join(paths.DIST, 'fonts')
                }
            ]
        })
    ],
    module: {
        rules: [
            {
                test: /\.(js|jsx)$/,
                exclude: /node_modules/,
                use: ['babel-loader', 'source-map-loader']
            },
            { test: /\.tsx?$/, loader: 'awesome-typescript-loader' }
        ]
    },
    resolve: {
        extensions: ['.js', '.jsx', '.ts', '.tsx']
    }
};
