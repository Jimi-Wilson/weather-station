function capitaliseFirstLetter(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
}


function WeatherBlock(props) {

    switch (props.type) {
        case "temperature":
            break;
        case "humidity":
            break;
        case "pressure":
            break;
        case "wind":
            break;
        case "rain":
            break;

        default:
            break;

    }

    return (
        <div>
            <p>{capitaliseFirstLetter(props.type)}</p>
            <p>{props.value}</p>
        </div>
    )
}


export default WeatherBlock;