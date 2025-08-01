import WeatherBlock from "./WeatherBlock.jsx";
import {Thermometer, Gauge, Droplets} from "lucide-react";

function WeatherBlocks({latestWeatherData}){

    if (!latestWeatherData) {
        return;
    }
    const timestamp = new Date(latestWeatherData.timestamp);

    return (
        <div>
            <h1 className="text-5xl">Current Weather</h1>
            <p>Last Updated at {timestamp.toLocaleString()}</p>
        <div className="flex flex-col gap-10 flex-wrap sm:flex-row">
            <WeatherBlock type="temperature" value={latestWeatherData.temperature} icon={Thermometer} />
            <WeatherBlock type="humidity" value={latestWeatherData.humidity} icon={Droplets}/>
            <WeatherBlock type="pressure" value={latestWeatherData.pressure} icon={Gauge}/>
        </div>
        </div>

    )

}

export default WeatherBlocks;