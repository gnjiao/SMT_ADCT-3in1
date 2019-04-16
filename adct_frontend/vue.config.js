module.exports = {
    devServer: {
        open: true,
        host: '127.0.0.1',
        port: 3030,
        proxy: {
            '/api': {
                target: '<url>',
                ws: true,
                changOrigin: true
            }
        }
    }
};