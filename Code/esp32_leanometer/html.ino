const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
    <html>
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
                body { text-align: center; margin-top: 20%; }
                button { display: block; margin: 10px auto; padding: 8px 4px; min-width: 152px;}
            </style>
        </head  >
        <body onload="getRTC()">
            
            <div>
                <h2>Leanometer</h2>
                <p id="time-p"></p>
                <button onclick="syncRTC();">Sync RTC</button>
                <button onclick="calibrateIMU();">Calibrate IMU</button>
                <button onclick="downloadLog();">Download Last Log</button>
            </div>

            <script>
                async function syncRTC() {
                    let currentTime = new Date();
                    let json = {
                        "Y": currentTime.getFullYear(),
                        "M": currentTime.getMonth() + 1,
                        "D": currentTime.getDate(),
                        "h": currentTime.getHours(),
                        "m": currentTime.getMinutes(),
                        "s": currentTime.getSeconds()
                    };
                    try {
                        const response = await fetch('/syncRTC', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/json'
                            },
                            body: JSON.stringify(json)
                            });
                        if (response.ok) {
                            alert("RTC sync!");
                            location.reload();
                        } else {
                            alert("Failed to sync RTC");
                        }
                    } 
                    catch (error) {
                        console.error('Error:', error);
                        alert("An error occurred");
                    }
                }

                async function getRTC() {
                    try {
                        const response = await fetch('/getRTC', { method: 'GET' });
                        if (!response.ok) {
                            throw new Error('Failed to get RTC');
                        }
                        
                        const data = await response.json();
                        
                        const date = new Date(data['unixTime']*1000);
                        document.getElementById('time-p').innerText = 'MCU date/time: ' + date.toLocaleString('en-GB', {timeZone:'GMT'});
                    } catch (error) {
                        console.error('Error:', error);
                        alert('Failed to get RTC');
                    }
                }
                    
                async function calibrateIMU() {
                    try {
                        const response = await fetch('/calibrateIMU', { method: 'GET' });
                        if (!response.ok) {
                            throw new Error('Failed to calibrate IMU');
                        }
                        alert("Do NOT move sensor until angle is back");
                    } catch (error) {
                        console.error('Error:', error);
                        alert('An error occurred while trying to calibrate the IMU');
                    }
                }
                
            </script>
        </body>
    </html>
)rawliteral";
