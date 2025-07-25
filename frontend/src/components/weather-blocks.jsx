import WeatherBlock from "./weather-block.jsx";

function WeatherBlocks(props){

    return (
        <div>
            <WeatherBlock type="temperature" value={22}/>
        </div>
    )

}

export default WeatherBlocks;